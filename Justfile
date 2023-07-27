PDFLATEX_COMMAND := "pdflatex -shell-escape -interaction=batchmode -halt-on-error"
LATEXMK_COMMAND := "latexmk -shell-escape -pdf"

default:
	@just --list

# Generates all papers
papers: proposal

# Generates thesis proposal document from LaTeX
proposal:
	@cd papers/proposal && {{LATEXMK_COMMAND}} proposal.tex
