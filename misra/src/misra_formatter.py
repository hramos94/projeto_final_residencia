import os
import argparse

# Function to clean a report file
def clean_report(txt_file, prefix, file_base_name):
    print(f"Formatting: {txt_file}")
    
    with open(txt_file, 'r') as f:
        lines = f.readlines()

    # Filters the lines that belong to the .c and .h files
    filtered_lines = [
        line for line in lines
        if line.startswith(f'[{prefix}/') and (
            f'{file_base_name}.c' in line or f'{file_base_name}.h' in line
        )
    ]

    with open(txt_file, 'w') as f:
        f.writelines(filtered_lines)

# Function to iterate through all files in the directory
def clean_all_reports(report_dir):
    prefix = report_dir.split('/', 1)[1] if '/' in report_dir else report_dir

    for filename in os.listdir(report_dir):
        if filename.endswith(".txt"):
            # Get the base name of the file (without the .txt suffix)
            file_base_name = filename.rsplit('_', 1)[0]

            txt_file = os.path.join(report_dir, filename)
            
            # Clean the report file, keeping only .c and .h files
            clean_report(txt_file, prefix, file_base_name)

    print("Formatting Done")

# Entry point
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Clean MISRA report files.")
    parser.add_argument("report_dir", help="Path to the report directory (e.g., misra/bsw)")
    args = parser.parse_args()

    clean_all_reports(args.report_dir)