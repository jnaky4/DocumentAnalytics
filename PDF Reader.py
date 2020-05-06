import PyPDF2
FILE_PATH = './Bible.pdf'
savefile = open("./cmake-build-debug/import.txt", "a")

with open(FILE_PATH, 'rb') as f:
    reader = PyPDF2.PdfFileReader(f)
    for page in reader.pages:
        #print(page.extractText())
        savefile.writelines(page.extractText())
f.close()
savefile.close()


