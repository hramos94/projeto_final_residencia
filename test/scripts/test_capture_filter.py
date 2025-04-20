import sys

def filtrar_linhas(dados):
    """Filtra as linhas que começam com 'TEST(' ou são 'PASS'."""
    linhas = dados.splitlines()
    linhas_filtradas = [linha for linha in linhas if linha.strip().startswith("TEST(") or linha.strip() == "PASS"]
    return "\n".join(linhas_filtradas)

def processar_arquivo(arquivo_entrada, arquivo_saida):
    """Lê o arquivo de entrada, aplica o filtro e salva o resultado no arquivo de saída."""
    try:
        with open(arquivo_entrada, 'r', encoding='utf-8') as f:
            conteudo = f.read()
    except FileNotFoundError:
        print(f"Erro: O arquivo {arquivo_entrada} não foi encontrado.")
        sys.exit(1)

    resultado = filtrar_linhas(conteudo)

    try:
        with open(arquivo_saida, 'w', encoding='utf-8') as f:
            f.write(resultado)
        print(f"Linhas filtradas salvas em '{arquivo_saida}'")
    except Exception as e:
        print(f"Erro ao salvar o arquivo: {e}")
        sys.exit(1)

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Uso: python3 test_capture_cleaner.py <arquivo_entrada> <arquivo_saida>")
        sys.exit(1)
    
    arquivo_entrada = sys.argv[1]
    arquivo_saida = sys.argv[2]
    processar_arquivo(arquivo_entrada, arquivo_saida)
