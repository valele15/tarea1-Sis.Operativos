# tarea1-Sis.Operativos
plan.txt:
Plan de actividades diciocheras
parser:  que actvidades existen y de que depende.Para esto usamos el plan.txt que en este se encuentr todo en una linea,pero queremos clasificarlo en un plan como de actividades como sale en el enunciado.
Algo asi:   
-ID_Actividad: Identificador único alfanumérico del nodo.
- Nombre_Actividad: Etiqueta descriptiva de la acción.
- Tiempo (tiempo_ms): El tiempo estimado de la actividad en milisegundos.
- Dependencias: Lista de IDs separados por comas. Estas actividades deben haber finalizado antes de que el nodo actual pueda ejecutarse.
COmo una especie de lista y separarlo en vex dde hacerlo en una linea
*si el tiempo viene vacio ,asignar un valor aleatoria entre 100 y 5000 ms
