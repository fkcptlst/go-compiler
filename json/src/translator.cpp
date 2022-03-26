#include "translator.h"


void Translator::exitJson(JsonParser::JsonContext *ctx) {
    xml_.put(ctx, xml_.get(ctx->children[0]));
    ret_xml_ = xml_.get(ctx->children[0]);
}


void Translator::exitNotNullObject(JsonParser::NotNullObjectContext *ctx) {
    std::string object = "\n";
    for (JsonParser::PairContext *pair : ctx->pair()) {
        object += xml_.get(pair);
    }
    xml_.put(ctx, object);
}


void Translator::exitEmptyObject(JsonParser::EmptyObjectContext *ctx) {
    /* TODO: to make easier? */
    xml_.put(ctx, "");
}


void Translator::exitNotNullArray(JsonParser::NotNullArrayContext *ctx) {
    std::string array;
    for (JsonParser::ValueContext *value : ctx->value()) {
        array += "<element>" + xml_.get(value) + "</element>\n";
    }
    xml_.put(ctx, array);
}


void Translator::exitEmptyArray(JsonParser::EmptyArrayContext *ctx) {
    /* TODO: to make easier? */
    xml_.put(ctx, "");
}


void Translator::exitPair(JsonParser::PairContext *ctx) {
    /* a:b -> <a>b</a> */
    std::string str = ctx->STRING()->getText();
    std::string key = str.substr(1, str.length() - 2);
    std::string value = xml_.get(ctx->value());
    std::string xml = "<" + key + ">" + value + "</" + key + ">\n";
    xml_.put(ctx, xml);
}


void Translator::exitString(JsonParser::StringContext *ctx) {
    std::string str = ctx->getText();
    xml_.put(ctx, str.substr(1, str.length() - 2));
}


void Translator::exitAtom(JsonParser::AtomContext *ctx) {
    xml_.put(ctx, ctx->getText());
}


void Translator::exitObjectValue(JsonParser::ObjectValueContext *ctx) {
    xml_.put(ctx, xml_.get(ctx->object()));
}


void Translator::exitArrayValue(JsonParser::ArrayValueContext *ctx) {
    xml_.put(ctx, xml_.get(ctx->array()));
}
