go_source_dir=$1
output_dir=./tmp/gt

# 1. setup output dir
if [ ! -d $output_dir ]; then
  mkdir -p $output_dir
fi

# 2. replace my_print with fmt.Print
for file in "$go_source_dir"/*.go; do
    if [ -f "$file" ]; then
        output_file="$output_dir/$(basename "$file")"
        # replace myprint with fmt.Print
        sed '/func myprint(a int)/c\
import "fmt"\
func myprint(a int) {\
    fmt.Print(a)\
}' "$file" > "$output_file"

    fi
done

# 3. compile
cd $output_dir && find . -name "*.go" | xargs -I {} go build {}
