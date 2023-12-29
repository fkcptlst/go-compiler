# working directory: project root

# ANTLR 4
ANTLR_HOME="/usr/local/lib"
ANTLR_JAR="$ANTLR_HOME/antlr-4.13.1-complete.jar"
CLASSPATH=".:$ANTLR_JAR:$CLASSPATH"

cd go_compiler/grammar/

java -jar $ANTLR_JAR -Dlanguage=Python3 *.g4 -o generated

git reset --hard HEAD >/dev/null 2>&1
