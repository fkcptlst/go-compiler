# 加载 antlr4环境
base_dir=$(readlink -f ..)
alias antlr4="java -jar ${base_dir}/3rd/antlr-4.9.3-complete.jar"
export CLASSPATH=".:${base_dir}/3rd/antlr-4.9.3-complete.jar:$CLASSPATH"
alias grun="java org.antlr.v4.gui.TestRig"

# 处理 .g4文件
cd grammar
mkdir -p java
antlr4 *.g4 -o java/

cd java
javac *.java

# grun 的参数:
#   -tokens: 打印词法符号流
#   -tree:  打印语法分析树
#   -gui:   以对话框方式显示tree
#   -...

grun R prog -gui ../../../assets/R/1.R

cd ../..
rm -rf ./grammar/java
