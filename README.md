# Processamento Digital de Imagens - Unidade 1 (DCA0445)

Repositório com as resoluções dos exercícios práticos da Unidade 1 da disciplina DCA0445 (Processamento Digital de Imagens), ministrada pelo Prof. Agostinho Brito Jr.

**Aluno:** Eduardo Murilo Pinto Taborda (20220057376)

## Relatório Online (GitHub Pages)

O relatório completo com os códigos-fonte, discussões, imagens e vídeos gerados pode ser acessado no link abaixo:

[https://empt4.github.io/pdi-lista_exercicios_u1/](https://empt4.github.io/pdi-lista_exercicios_u1/)

## Estrutura do Repositório

| Diretório | Tema | Arquivos Principais |
| :--- | :--- | :--- |
| `exemplos/hello` | Teste inicial de ambiente | `hello.cpp` |
| `capitulo_2` | Manipulação de pixels | `regions.cpp`, `trocaregioes.cpp` |
| `capitulo_3` | Tratamento de arquivos de vídeo | `video_cinza.cpp`, `video_negativo.cpp` |
| `capitulo_4` | Perfis de intensidade e visualização 3D | `visualizacao.cpp` |
| `capitulo_5` | Serialização em ponto flutuante (FileStorage) | `filestorage_exercicio.cpp` |
| `capitulo_6` | Mapas de cores (Pseudocores e LUT) | `colormaps_padrao.cpp`, `colormap_personalizado.cpp` |
| `capitulo_7` | Espaços de cores (HSV) | `colorspace_mapas.cpp`, `colorspace_personalizado.cpp` |
| `capitulo_8` | Seleção interativa de pixels | `colorrange_pintura.cpp` |
| `capitulo_9` | Combinação de imagens (Light Painting) | `light_painting.cpp` |
| `capitulo_10` | Decomposição em planos de bits (Esteganografia) | `decodificador.cpp` |
| `capitulo_11` | Preenchimento de regiões (Floodfill e contagem) | `labeling_aprimorado.cpp` |
| `capitulo_12` | Manipulação de histogramas | `equalize.cpp`, `motiondetector.cpp` |
| `capitulo_13` | Registro de imagens e correção de perspectiva | `registro_interativo.cpp` |
| `capitulo_14` | Filtragem espacial I (Convolução e Focus Stacking) | `convolucao2.cpp`, `depthoffield.cpp` |
| `capitulo_15` | Filtragem espacial II (Tilt-Shift em imagem e vídeo) | `tiltshift.cpp`, `tiltshiftvideo.cpp` |

## Compilação e Execução

Os programas foram desenvolvidos em C++ utilizando OpenCV 4 e CMake no ambiente WSL (Ubuntu). Para compilar qualquer exercício individualmente:

```bash
cd capitulo_X
mkdir -p build && cd build
cmake ..
make