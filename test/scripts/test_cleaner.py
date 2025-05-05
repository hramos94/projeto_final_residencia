def clean_test_results_file(input_path='test/scripts/test_capture_filtered.txt', output_path='test/test_results.txt'):
    with open(input_path, 'r', encoding='utf-8') as f:
        lines = f.readlines()

    filtered_lines = []
    i = 0
    while i < len(lines):
        line = lines[i].strip()

        if line.startswith('PASS'):
            # Standalone PASS line — keep it
            filtered_lines.append(lines[i])
            i += 1

        elif line.startswith('TEST('):
            test_name = line.split(')')[0] + ')'
            has_inline_pass = line.endswith('PASS') or line.endswith('PASS\n') or ' PASS' in line

            if has_inline_pass:
                # Line already includes PASS — rewrite without extra message
                filtered_lines.append(test_name + ' PASS\n')
                i += 1
            else:
                # Check if next line is an isolated PASS
                next_line = lines[i + 1].strip() if i + 1 < len(lines) else ''
                if next_line == 'PASS':
                    filtered_lines.append(test_name + ' PASS\n')
                    i += 2  # skip the PASS line too
                else:
                    # No PASS confirmation — just keep the clean test name
                    filtered_lines.append(test_name + '\n')
                    i += 1
                    # Skip intermediate logs
                    while i < len(lines) and not lines[i].startswith('TEST(') and not lines[i].startswith('PASS'):
                        i += 1
        else:
            i += 1  # ignore any other line

    with open(output_path, 'w', encoding='utf-8') as f:
        f.writelines(filtered_lines)

# Run
clean_test_results_file()
