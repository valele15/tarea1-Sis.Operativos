archivo = open("plan_grande.txt", "w")

for i in range(1, 10001):

    if i == 1:
        archivo.write(f"{i} : actividad_{i} : 100 :\n")
    else:
        archivo.write(f"{i} : actividad_{i} : 100 : {i-1}\n")

archivo.close()

print("Archivo creado correctamente.")
