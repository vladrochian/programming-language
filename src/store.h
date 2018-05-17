#ifndef BEAUTY_LANG_STORE_H
#define BEAUTY_LANG_STORE_H

#include <exception>
#include <map>
#include <string>
#include <vector>

#include "types.h"
#include "value.h"

class ObjectData {
  public:
    enum Type {
        VARIABLE,
        FUNCTION
    };
    virtual Type getType() const = 0;
};

class VariableData : public ObjectData {
  public:
    explicit VariableData(PrimitiveType type);
    Type getType() const override;
    PrimitiveType getVariableType() const;
    bool getBooleanValue() const;
    double getNumberValue() const;
    std::string getStringValue() const;
    void setValue(bool value);
    void setValue(double value);
    void setValue(const std::string& value);

  private:
    PrimitiveType type;
    bool booleanValue;
    double numberValue;
    std::string stringValue;
};

class StackLevel {
  public:
    void registerName(const std::string& name, const ObjectData& objectData);
    ObjectData* lookupName(const std::string& name) const;

  private:
    std::map<std::string, ObjectData> names;
};

class Store {
  public:
    void registerName(const std::string& name, const ObjectData& objectData);
    Rvalue getValue(const std::string& name) const;
    void setValue(const std::string& name, const Value& value);
    void newLevel();
    void deleteLevel();

  private:
    VariableData* getVariableData(const std::string& name) const;
    std::vector<StackLevel> stk;
};

Store store;

#endif //BEAUTY_LANG_STORE_H
