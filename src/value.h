//#ifndef BEAUTY_LANG_VALUE_H
//#define BEAUTY_LANG_VALUE_H
//
//#include <string>
//#include <map>
//
//class Value {
//  public:
//    typedef std::map<std::string, Value> ObjectValue;
//    explicit Value(double numberValue) : type("number"), value(numberValue) {}
//    explicit Value(bool booleanValue) : type("boolean"), value(booleanValue) {}
//    explicit Value(std::string stringValue) : type("string"), value(std::move(stringValue)) {}
//    explicit Value(ObjectValue objectValue, std::string type = "Object")
//            : type(std::move(type)), value(std::move(objectValue)) {}
//  private:
//    union ValueUnion {
//        explicit ValueUnion(double numberValue) : numberValue(numberValue) {}
//        explicit ValueUnion(bool booleanValue) : booleanValue(booleanValue) {}
//        explicit ValueUnion(std::string stringValue) : stringValue(std::move(stringValue)) {}
//        explicit ValueUnion(ObjectValue objectValue) : objectValue(std::move(objectValue)) {}
//        double numberValue;
//        bool booleanValue;
//        std::string stringValue;
//        ObjectValue objectValue;
//    } value;
//    std::string type;
//};
//
//#endif //BEAUTY_LANG_PRIMITIVE_VALUE_H
