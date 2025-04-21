import os
import re
from collections import defaultdict

# Paths
RULES_PATH = 'misra_c_2012.txt'
MISRA_DIR = 'misra/'
OUTPUT_PATH = os.path.join(MISRA_DIR, 'misra_summary.txt')

# Custom violation level order
VIOLATION_LEVELS = ['Mandatory', 'Required', 'Advisory', 'Unknown']

# Load MISRA rules and their levels
def load_rules(path):
    rules = {}
    with open(path, 'r', encoding='utf-8') as file:
        for line in file:
            parts = line.strip().split('\t')
            if len(parts) >= 2:
                rule_id = parts[0].replace('Rule ', '').strip()
                level = parts[1].strip()
                rules[f'misra-c2012-{rule_id}'] = level
    return rules

# Process violations in a single file
def process_file(path, rules, total_violations):
    local_counter = defaultdict(int)
    with open(path, 'r', encoding='utf-8') as file:
        for line in file:
            match = re.search(r'\[style - (misra-c2012-\d+\.\d+)\]', line)
            if match:
                rule = match.group(1)
                level = rules.get(rule, 'Unknown')
                local_counter[level] += 1
                total_violations[level] += 1
    return local_counter

# Main function
def main():
    if not os.path.isfile(RULES_PATH):
        print(f"Error: File '{RULES_PATH}' not available.")
        return

    rules = load_rules(RULES_PATH)
    total_violations = defaultdict(int)
    files_by_subfolder = defaultdict(list)

    # Walk through all subfolders (bsw, reb, aux)
    for root, _, files in os.walk(MISRA_DIR):
        for filename in files:
            if filename.endswith('.txt') and filename != 'misra_summary.txt':
                filepath = os.path.join(root, filename)
                count = process_file(filepath, rules, total_violations)
                rel_path = os.path.relpath(filepath, MISRA_DIR)
                folder = rel_path.split(os.sep)[0].upper()
                files_by_subfolder[folder].append((rel_path, count))

    # Build formatted output
    output_lines = []
    for section in ['BSW', 'REB', 'AUX']:
        if section in files_by_subfolder:
            output_lines.append(f"{'=' * 10} {section} {'=' * 10}")
            for rel_path, count in files_by_subfolder[section]:
                output_lines.append(f"File: {rel_path}")
                for level in VIOLATION_LEVELS:
                    output_lines.append(f"  {level}: {count[level]}")
                output_lines.append("")  # blank line

    # Total summary
    output_lines.append("=" * 10 + " TOTAL " + "=" * 10)
    for level in VIOLATION_LEVELS:
        output_lines.append(f"{level}: {total_violations[level]}")

    with open(OUTPUT_PATH, 'w', encoding='utf-8') as out_file:
        out_file.write('\n'.join(output_lines))

    print(f"Formatted summary generated at: {OUTPUT_PATH}")

if __name__ == '__main__':
    main()