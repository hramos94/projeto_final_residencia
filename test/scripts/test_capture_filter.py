import sys

def filter_lines(data):
    """Filters lines that start with 'TEST(' or are exactly 'PASS'."""
    lines = data.splitlines()
    filtered_lines = [line for line in lines if line.strip().startswith("TEST(") or line.strip() == "PASS"]
    return "\n".join(filtered_lines)

def process_file(input_file, output_file):
    """Reads the input file, applies the filter, and saves the result to the output file."""
    try:
        with open(input_file, 'r', encoding='utf-8') as f:
            content = f.read()
    except FileNotFoundError:
        print(f"Error: The file {input_file} was not found.")
        sys.exit(1)

    result = filter_lines(content)

    try:
        with open(output_file, 'w', encoding='utf-8') as f:
            f.write(result)
        print(f"Filtered lines saved to '{output_file}'")
    except Exception as e:
        print(f"Error while saving the file: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 test_capture_cleaner.py <input_file> <output_file>")
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = sys.argv[2]
    process_file(input_file, output_file)