#ifndef JSON_INCLUDE_TRANSLATOR_H_
#define JSON_INCLUDE_TRANSLATOR_H_


#include "JsonBaseListener.h"


class Translator : public JsonBaseListener {

public:
    std::string ret_xml_;

    /* json */
    void exitJson(JsonParser::JsonContext *ctx);

    /* object */
    void exitNotNullObject(JsonParser::NotNullObjectContext *ctx);
    void exitEmptyObject(JsonParser::EmptyObjectContext *ctx);

    /* array */
    void exitNotNullArray(JsonParser::NotNullArrayContext *ctx);
    void exitEmptyArray(JsonParser::EmptyArrayContext *ctx);

    /* pair */
    void exitPair(JsonParser::PairContext *ctx);

    /* value */
    void exitString(JsonParser::StringContext *ctx);
    void exitAtom(JsonParser::AtomContext *ctx);
    void exitObjectValue(JsonParser::ObjectValueContext *ctx);
    void exitArrayValue(JsonParser::ArrayValueContext *ctx);

private:
    antlr4::tree::ParseTreeProperty<std::string> xml_;
};


#endif