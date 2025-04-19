import os
import re
import csv

FOLDERS_TO_SCAN = {
    "test/reb/src": "reb",
    "test/aux/src": "aux"
}
CSV_FILE = "test/test_report.csv"

# Patterns
comment_block_pattern = re.compile(r"/\*\*(.*?)\*/", re.DOTALL)
test_macro_pattern = re.compile(r"(?<!SETUP)(?<!TEAR_DOWN)\bTEST\s*\(\s*([^\s,]+)\s*,\s*([^)]+)\)")
description_pattern = re.compile(r"@brief\s+(.*)")
requirements_pattern = re.compile(r"@requir\{(.*?)\}")

def extract_multiline_field(block, label):
    lines = block.splitlines()
    capture = False
    content = []
    for line in lines:
        line = line.strip()
        if line.startswith(f"* {label}:"):
            capture = True
            continue
        if capture:
            if line.startswith("*  -"):
                clean = re.sub(r"^\*  -\s*", "", line)
                content.append(clean)
            else:
                break
    return " ".join(content).strip()

test_data = []

for folder, origin in FOLDERS_TO_SCAN.items():
    for root, _, files in os.walk(folder):
        for file in files:
            if file.endswith(".c"):
                file_path = os.path.join(root, file)
                with open(file_path, encoding="utf-8") as f:
                    content = f.read()

                    comment_blocks = [(m.start(), m.group(1)) for m in comment_block_pattern.finditer(content)]
                    test_macros = [(m.start(), m.group(1).strip(), m.group(2).strip())
                                   for m in test_macro_pattern.finditer(content)]

                    for start_comment, comment_text in comment_blocks:
                        matching_test = next(((s, g, n) for s, g, n in test_macros if s > start_comment), None)
                        if not matching_test:
                            continue

                        _, test_group, test_name = matching_test

                        description = description_pattern.search(comment_text)
                        requirements = requirements_pattern.findall(comment_text)

                        description = description.group(1).strip() if description else ""
                        scenario = extract_multiline_field(comment_text, "Scenario")
                        expected = extract_multiline_field(comment_text, "Expected")
                        requirements = ",".join(requirements)

                        # Ignore incomplete tests
                        if not scenario and not expected:
                            continue

                        test_data.append([
                            origin,
                            file,
                            test_group,
                            test_name,
                            requirements,
                            description,
                            scenario,
                            expected
                        ])

# Write to CSV
with open(CSV_FILE, mode="w", newline='', encoding="utf-8") as csvfile:
    writer = csv.writer(csvfile, delimiter=';')
    writer.writerow([
        "Test_origin", "Test_location", "Test_group", "Test_name",
        "Requirements", "Description", "Scenario", "Expected Outcome"
    ])
    writer.writerows(test_data)

print(f" CSV '{CSV_FILE}' generated.")
