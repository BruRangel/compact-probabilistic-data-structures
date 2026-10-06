# Nome do arquivo principal (sem a extensão .tex)
DOC = main

all: $(DOC).pdf

$(DOC).pdf: $(DOC).tex
	latexmk -pdf -synctex=1 -interaction=nonstopmode $(DOC).tex

# Limpa os arquivos temporários de compilação (.aux, .log, etc)
clean:
	latexmk -c

# Limpa os temporários e também o PDF gerado
purge:
	latexmk -C