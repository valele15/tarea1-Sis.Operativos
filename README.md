# tarea1-Sis.Operativos (revisar explicacion por si me salte algo )
plan.txt:
Plan de actividades diciocheras


parser:  que actvidades existen y de que depende.Para esto usamos el plan.txt que en este se encuentr todo en una linea,pero queremos clasificarlo en un plan como de actividades como sale en el enunciado.


Algo asi:   

- ID_Actividad: Identificador único alfanumérico del nodo.
- Nombre_Actividad: Etiqueta descriptiva de la acción.
- Tiempo (tiempo_ms): El tiempo estimado de la actividad en milisegundos.
- Dependencias: Lista de IDs separados por comas. Estas actividades deben haber finalizado antes de que el nodo actual pueda ejecutarse.


COmo una especie de lista y separarlo en vex dde hacerlo en una linea
*si el tiempo viene vacio ,asignar un valor aleatoria entre 100 y 5000 ms

planificador.cpp :

epara esta parte segui el formato de plan.txt osea lo fui hjaciendo como en especies de columnas y deje los datos como colocados de esa manera. ID : Nombre : tiempo_ms : dependencias   de esta forma.  Para aquello tengo que ir leyendo el archivo por id,nombre,tiempo_ms y dependencias por lo que separe cada uno con un : para guardar todo como en una estructura ,como una esecie de lista o actividad. Luego de hacer eso el id y nombre se guardaban automaticamente ,mientras que el tiempo se convertia en texto a numero con stoi y se ponia como mencione antes que si tenia vacio se generaba uno aleatoriamente ente 100 y 5000. Las dependencias como vienen juntas ,simplemente guardamso cada una poniendole entre medio una coma para asi guradarlas cada un a por separada en un vector
lo de los numeros aleatorios se hace con srand que hace que permita asignarle un tiempo en este caso le agregamos tiempo nulo para que sea mas aleatorio en vez de que salga el mismo numero a cada rato. Tambien en el codigo abrimos ek plan.txt para poder leerlo y seguir el formato. guardamos todo en una misma linea que luego vaa a tner una estructura como de una lista.
para el tiempo simplemente se ahcen ambos casos necesrio el que es vacio y el viene con un tiempo incluido 

