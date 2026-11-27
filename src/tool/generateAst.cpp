#include <algorithm>
#include <cctype>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define ENDL '\n'

// TODO: ' ' delim으로 쓰는 토크나이저 별도로 구현하기?
// TODO: accept 등에서 템플릿 대신 void*로 반환하도록 변경하기.
// TODO: 디렉터리 원래 상태로 다 되돌려놓기.

// code from https://stackoverflow.com/questions/216823/how-can-i-trim-a-stdstring
inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
}
inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
}
inline void trim(std::string& s) {
    ltrim(s);
    rtrim(s);
}


struct Field {
    std::string type, name;
    Field(std::string type, std::string name): type(type), name(name)
    {}
    Field(std::string field) {
        std::stringstream fieldStream(field);
        std::string buffer;
        getline(fieldStream, buffer, ' ');
        trim(buffer);
        type = buffer;
        getline(fieldStream, buffer);
        trim(buffer);
        name = buffer;
    }
};

struct classScaffold {
    std::string className;
    std::string parentClassName;
    std::vector<Field> fields;
    std::vector<classScaffold> subclasses;

    // class className: public baseName {
    // public:
    std::string classOpen() const {
        std::stringstream ss;

        if(!parentClassName.empty()) {
            ss << "class " << parentClassName << "::" << className << ": public " << parentClassName;
        }
        else {
            ss << "class " << className;
        }
        ss << " {\n";
        ss << "public:\n";

        return ss.str();
    }

    // className(Field1 *field1, ...): field1(field1), ... {}
    std::string buildConstructor() const {
        std::stringstream ss;

        // parameters
        ss << className << '(';
        for(int i=0;i<fields.size();i++) {
            if(fields[i].type == "void*") {
                ss << fields[i].type << " " << fields[i].name;
            } else {
                ss << fields[i].type << " *" << fields[i].name;
            }
            if(i + 1 != fields.size()) ss << ", ";
        }
        ss << ')';

        // field init
        if(!fields.empty()) {
            ss << ": ";
            for(int i=0;i<fields.size();i++) {
                ss << fields[i].name << '(' << fields[i].name << ')';
                if(i + 1 != fields.size()) ss << ", ";
            }
        }
        ss << "{}\n";

        return ss.str();
    }

    // Field1 *field1; Field2 *field2; ...
    std::string buildFields() const {
        std::stringstream ss;
        for(const Field& field : fields) {
            if(field.type == "void*") {
                ss << field.type << " " << field.name << ";\n";
            } else {
                ss << field.type << " *" << field.name << ";\n";
            }
        }
        return ss.str();
    }

    // class Class1; ...
    std::string buildSubclasses() const {
        std::stringstream ss;
        for(const auto& subclass : subclasses) {
            ss << "class " << subclass.className << ";\n";
        }
        return ss.str();
    }

    std::string buildVisitorClass() const {
        return "class Visitor;\n";
    }

    std::string buildVisitorAccept() const {
        return "virtual void* accept(Visitor *visitor);\n";
    }

    std::string buildVisitorAcceptConcrete() const {
        std::stringstream ss;

        ss << "void* accept(Visitor *visitor) override {\n";
        ss << "return visitor->visit" << className << parentClassName << "(this);\n";
        ss << "}\n";

        return ss.str();
    }

    std::string classClose() const {
        std::stringstream ss;
        ss << "};";
        return ss.str();
    }

    public:
    classScaffold(std::string className, std::vector<Field> fields):
        classScaffold(className, "", fields, {})
    {}
    classScaffold(std::string className, std::vector<classScaffold> subclasses):
        classScaffold(className, "", {}, subclasses)
    {}
    classScaffold(std::string className, std::string parentClassName, std::vector<Field> fields):
        classScaffold(className, parentClassName, fields, {})
    {}
    classScaffold(std::string className, std::string parentClassName, std::vector<Field> fields, std::vector<classScaffold> subclasses):
        className(className),
        parentClassName(parentClassName),
        fields(fields),
        subclasses(subclasses)
    {}

    std::string defineClass() const {
        std::stringstream ss;
        ss << classOpen();
        ss << buildConstructor();
        ss << buildFields();
        ss << buildSubclasses();
        ss << buildVisitorClass();
        ss << buildVisitorAccept();
        ss << classClose();
        return ss.str();
    }

    std::string defineSubClass() const {
        std::stringstream ss;
        ss << classOpen();
        ss << buildConstructor();
        ss << buildFields();
        ss << buildVisitorAcceptConcrete();
        ss << classClose();
        return ss.str();
    }

    // only for parent class
    std::string defineVisitor() const {
        std::stringstream ss;

        ss << "class " << className << "::Visitor {" << ENDL;
        ss << "public:" << ENDL;

        for(const classScaffold subclass : subclasses) {
            std::string lowClassName = subclass.className;
            lowClassName[0] = tolower(lowClassName[0]);
            ss << "virtual void* visit" << subclass.className << className
                << "(typename " << className << "::" << subclass.className << " *expr);" << ENDL;
        }

        ss << "};" << ENDL;

        return ss.str();
    }
};


std::vector<Field> buildFields(const std::string& fieldList) {
    std::stringstream fieldstream(fieldList);
    std::vector<Field> fields;
    std::string field;
    while(std::getline(fieldstream, field, ',')) {
        trim(field);
        fields.push_back(Field(field));
    }
    return fields;
}

void defineAst(const std::string& outputDir, const std::string& baseName, const std::vector<std::string> types) {
    std::string path = outputDir + "/" + baseName + ".h";
    std::cout << "saving path: " << path << std::endl;

    std::ofstream fs(path);
    if(!fs.is_open()) {
        std::cerr << "fstream 생성 실패" << std::endl;
        exit(1);
    }

    fs << "#pragma once\n";
    fs << "#include \"Lox.h\"\n";

    std::vector<classScaffold> subclasses;
    for(const std::string& type : types) {
        std::stringstream ss(type); // className: Field1 field1, ...

        std::string className, fields;
        std::getline(ss, className, ':');
        std::getline(ss, fields); // Field1 field1, ...

        trim(className);
        trim(fields);

        const std::vector<Field> fieldList = buildFields(fields);
        subclasses.push_back(classScaffold(className, baseName, fieldList));
    }

    // visitor 정의
    fs << "class Visitor;" << ENDL;

    // base class 생성
    const classScaffold baseClass = classScaffold("Expr", subclasses);
    fs << baseClass.defineClass() << ENDL;

    // visitor 생성
    fs << baseClass.defineVisitor() << ENDL;

    // base class 상속한 classes 생성
    for(const auto& subclass : subclasses) {
        fs << subclass.defineSubClass() << ENDL;
    }

}

int main(int argc, char** argv) {
    if(argc != 2) {
        std::cout << "Usage: generateAst <output directory>";
        exit(64);
    }

    std::string outputDir = argv[1];
    std::cout << "outputDir: " << outputDir << std::endl;

    // operator는 cpp 예약어라서 사용 불가
    defineAst(outputDir, "Expr", std::vector<std::string> {
        "Binary     : Expr left, Token op, Expr right",
        "Grouping   : Expr expression",
        "Literal    : void* value",
        "Unary      : Token op, Expr right",
    });
}
