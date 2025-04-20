def limpar_resultados_arquivo(entrada='test/scripts/test_capture_filtered.txt', saida='test/test_results.txt'):
    with open(entrada, 'r', encoding='utf-8') as f:
        linhas = f.readlines()

    linhas_filtradas = []
    i = 0
    while i < len(linhas):
        linha = linhas[i].strip()

        if linha.startswith('PASS'):
            # Linha PASS sozinha — mantemos
            linhas_filtradas.append(linhas[i])
            i += 1

        elif linha.startswith('TEST('):
            nome_teste = linha.split(')')[0] + ')'
            tem_pass_inline = linha.endswith('PASS') or linha.endswith('PASS\n') or ' PASS' in linha

            if tem_pass_inline:
                # Linha já inclui PASS — reescreve sem mensagem adicional
                linhas_filtradas.append(nome_teste + ' PASS\n')
                i += 1
            else:
                # Verifica se próxima linha é um PASS isolado
                proxima_linha = linhas[i + 1].strip() if i + 1 < len(linhas) else ''
                if proxima_linha == 'PASS':
                    linhas_filtradas.append(nome_teste + ' PASS\n')
                    i += 2  # pula também a linha PASS
                else:
                    # Sem confirmação de PASS — só mantém o nome do teste limpo
                    linhas_filtradas.append(nome_teste + '\n')
                    i += 1
                    # Pula logs intermediários
                    while i < len(linhas) and not linhas[i].startswith('TEST(') and not linhas[i].startswith('PASS'):
                        i += 1
        else:
            i += 1  # ignora qualquer outra linha

    with open(saida, 'w', encoding='utf-8') as f:
        f.writelines(linhas_filtradas)

# Executa
limpar_resultados_arquivo()
