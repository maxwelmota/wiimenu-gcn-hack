# Wii Menu GCN Hack — Enhanced

**Suporte ao controle de GameCube no Menu do Nintendo Wii por meio do Priiloader**, com melhorias no analógico, botão START como HOME e coexistência com o Wii Remote.

> **Modificação comunitária do projeto [dg4l/wiimenu-gcn-hack](https://github.com/dg4l/wiimenu-gcn-hack); não é uma versão oficial do autor.** Agradecimentos especiais ao **dg4l** pela criação e disponibilização do hack original e ao **koopthekoopa** pelo projeto de descompilação [wii-ipl](https://github.com/koopthekoopa/wii-ipl), também reconhecido pelo autor original.

[Read in English](README.md)

## Novas funcionalidades

- **Zona morta (deadzone) no analógico esquerdo:** limite inicial de ±12 por eixo, para evitar movimentação involuntária da setinha quando o analógico está em repouso, sem prejudicar o uso observado.
- **START do GameCube → HOME do Wii:** no **Wii System Menu**, START abre a tela HOME.
- **Coexistência entre Wii Remote e GameCube Controller (GCC):** dois cursores independentes, com possibilidade de apontar e selecionar canais com ambos os dispositivos.
- **Atribuição dinâmica P1/P2:** se o GCC estiver sozinho, pode assumir P1. Quando um Wii Remote entra como P1, o GCC pode assumir P2; quando o Wii Remote é desligado, o GCC pode voltar automaticamente a P1.
- **Botão A no System Menu:** permite selecionar e abrir canais normalmente, preservando o mapeamento do projeto original.

## Atenção: o que o hack NÃO faz

**START não funciona como HOME dentro dos jogos de Wii.** O hack age **somente no Menu do Sistema do Wii**. Ao abrir um jogo de Wii compatível com controle GameCube, o controle volta a operar **normalmente como GameCube Controller**, inclusive com o START tendo sua função comum dentro do jogo. Esta modificação **não** injeta um botão HOME nos jogos, nem modifica suas rotinas internas ou cria um atalho universal para retornar ao Menu do Wii.

Da mesma forma, a existência de dois cursores independentes no **System Menu** não significa suporte simultâneo a dois jogadores em todos os jogos e aplicativos. O Homebrew Channel, USB Loader GX e emuladores utilizam seus próprios sistemas de controle. No teste realizado, o USB Loader GX tratou ambos como P1 dentro da sua interface; em *Tatsunoko vs. Capcom*, o controle GameCube funcionou nativamente, mas o uso simultâneo dos dois dispositivos não foi confirmado.

## Compatibilidade e status

| Item | Resultado |
| --- | --- |
| Wii System Menu **4.3U (v513)** | **Testado em Wii real** |
| Wii System Menu **4.3E (v514)** | Compilado e verificado; **não testado em hardware real** |
| Wii System Menu **4.3J (v512)** | Compilado e verificado; **não testado em hardware real** |
| Wii System Menu **4.3K** | Não suportado |
| Analógico sem drift | Testado e aprovado no Wii |
| GCC START como HOME, no Menu do Sistema | Testado e aprovado no Wii |
| GCC A seleciona e abre canais | Testado e aprovado no Wii |
| Dois cursores independentes (Wii Remote + GCC) | Testado e aprovado no Wii |
| GCC P1 ↔ P2 dinamicamente | Testado e aprovado no Wii |
| GCC START como HOME *dentro dos jogos* | **Não implementado** |

Esta é uma versão **beta comunitária**. Faça backup dos arquivos e mantenha um método funcional de recuperação (Priiloader pelo botão RESET e, preferencialmente, BootMii com backup da NAND).

## Arquivos para instalar

Na raiz do repositório, há **somente dois arquivos INI para instalação**:

| Arquivo | Uso |
| --- | --- |
| [`gcn.ini`](gcn.ini) | **Somente o hack**: acrescentar ao final do seu `hacks_hash.ini`. **Recomendado**, pois preserva os hacks que você já usa. |
| [`hacks_hash.ini`](hacks_hash.ini) | **Arquivo completo pré-mesclado**, com uma coleção específica de hacks já existente e a nossa modificação incluída. Não é um substituto universal para qualquer instalação do Priiloader. |

### Opção 1 — acrescentar o hack (recomendada)

1. Faça backup do seu `sd:/apps/priiloader/hacks_hash.ini`.
2. Abra esse arquivo em um editor de texto simples.
3. Verifique que a última linha do hack anterior termina com uma **quebra de linha** e que `[GameCube Controller Support]` começa **imediatamente na linha seguinte**, sem linha em branco e sem espaços entre os blocos. Se o arquivo já termina com `\n` (uma quebra de linha), não acrescente outra.
4. Copie **todo** o conteúdo de [`gcn.ini`](gcn.ini) para o final do arquivo. Ele inclui os blocos regionais 4.3U/4.3E/4.3J. Caso você já possua blocos chamados `[GameCube Controller Support]`, **substitua-os** em vez de duplicá-los.
5. Salve no mesmo caminho, como `hacks_hash.ini` (sem uma extensão `.txt` extra).
6. Reinicie, entre no Priiloader, abra **System Menu Hacks**, habilite **GameCube Controller Support** e escolha **Save settings**.
7. Teste com o controle de GameCube conectado à **porta 1**.

### Opção 2 — usar o arquivo pronto

Após fazer backup, copie diretamente o [`hacks_hash.ini`](hacks_hash.ini) do repositório para `sd:/apps/priiloader/hacks_hash.ini` e habilite o hack no Priiloader. **Esta opção pode sobrescrever outros hacks pessoais**; por isso recomendamos a Opção 1.

**Se o hack não aparecer:** confira o caminho, a versão regional do Menu do Wii e a separação exata entre os blocos: **uma quebra de linha, sem linha vazia**. Nos testes com este Wii/Priiloader, tanto a ausência da quebra de linha quanto uma linha em branco entre as seções impediram a exibição do hack.

**Se o System Menu falhar:** use o seu procedimento de recuperação (normalmente segurar RESET ao ligar para abrir o Priiloader), desabilite o hack e restaure o backup. Não experimente alterações no Menu do Sistema sem recuperação disponível.

## Desenvolvimento e compilação

O código-fonte e as ferramentas estão em [`source/`](source/). **Não é necessário compilar para instalar os arquivos INI fornecidos.**

Requisitos: `clang` com suporte PowerPC, `ld.lld`, `gcc`, `g++` com `std::format` e Python 3.

```bash
cd source
bash build-with-clang.sh
python3 test_gcnhook.py
cmp gcn.ini ../gcn.ini
```

O procedimento gera `source/gcn.ini` e valida os três patches. Os arquivos ELF são intermediários da compilação, **não devem ser instalados no Wii**.

## Créditos e licença

- **dg4l:** autor do [wiimenu-gcn-hack original](https://github.com/dg4l/wiimenu-gcn-hack). O hack base, a injeção de código e o suporte inicial ao controle GameCube são mérito do autor original.
- **koopthekoopa:** responsável pelo [wii-ipl](https://github.com/koopthekoopa/wii-ipl), projeto de descompilação fundamental, também agradecido no repositório original.
- **Contribuição comunitária:** deadzone, START → HOME no System Menu e coexistência dinâmica de controles, com testes em hardware Wii real.

A licença original **MIT** e o aviso de copyright foram preservados integralmente em [LICENSE](LICENSE). Informações adicionais em [CREDITS.md](CREDITS.md). Recomenda-se publicar como **fork**, preservando a relação com o repositório original no GitHub.

## Contribuições futuras

São bem-vindos relatos especialmente sobre 4.3E/4.3J, quatro controles Wii Remote conectados, troca de controle durante botões pressionados e estabilidade prolongada. Consulte [docs/TESTING.md](docs/TESTING.md).
