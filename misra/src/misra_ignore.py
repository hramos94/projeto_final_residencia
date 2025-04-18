import os

def load_ignore(ignore_file):
    """Loads ignore lines up to the '=>'"""
    ignored = set()
    with open(ignore_file, 'r') as f:
        for line in f:
            line = line.strip()
            if line and not line.startswith("#"):  # Ignore empty lines and comments
                # Add up to the '=>' to the ignored list
                true_index = line.find('=>') + 2
                if true_index != -1:
                    ignored.add(line[:true_index].strip())

    # Debug: Show loaded ignore lines
    # print(f"Ignored lines loaded: {ignored}")
    return ignored

def process_files(bsw_folder, ignore_file):
    """Processes the files in the bsw folder and removes lines based on the ignore list"""
    ignored = load_ignore(ignore_file)
    
    for root, _, files in os.walk(bsw_folder):
        for file in files:
            if file.endswith('.txt'):
                full_path = os.path.join(root, file)
                removed_lines = 0

                # Process file line by line
                with open(full_path, 'r') as f:
                    lines = f.readlines()

                # Filter out lines that are in the ignored list
                with open(full_path, 'w') as f:
                    for line in lines:
                        line_to_compare = line.strip()
                        # Check if the part up to '=>' is in the ignored list
                        if any(line_to_compare.startswith(ignored_item) for ignored_item in ignored):
                            removed_lines += 1
                            # Debug: Show removed line
                            # print(f"Removed line: {line.strip()}")
                        else:
                            f.write(line)

                print(f"Formatting: {full_path}")
                print(f"{removed_lines} ignored from {full_path}")

# Main function to run the script
def main():
    bsw_folder = 'misra/bsw'
    ignore_file = 'misra/src/misra_ignore.txt'
    process_files(bsw_folder, ignore_file)

if __name__ == "__main__":
    main()
