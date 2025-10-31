# Proyecto académico – Estructura de Datos (Los Booleanos)
Asignatura ESTRUCTURA DE DATOS período 2º Semestre 2023

**Integrantes:**  
Alonso Maurel  
Monserrath Morales  
Felipe Nuñez  
Francisco Quiroga

## Descripción

Aplicación desarrollada en lenguaje C para la gestión de usuarios en el área de la salud. El sistema permite:

- Crear y almacenar usuarios
- Asignar planes médicos y beneficios
- Persistir datos entre ejecuciones mediante archivos
- Evitar duplicación de usuarios ya registrados

## Estructuras utilizadas

- `HashMap` para gestión eficiente de usuarios
- `List` para almacenamiento dinámico
- Archivos `.txt` para persistencia de datos

## Cómo ejecutar

1. Clona el repositorio:
   ```bash
   git clone https://github.com/ZZZZIK/Proyecto_Estructura_LosBooleanos

2. Abre el proyecto en tu editor preferido (Visual Studio Code, Code::Blocks, etc.)
3. Compila el programa: gcc main.c hashmap.c list.c -o salud
4. Ejecuta el programa: ./salud (Requiere tener instalado GCC o cualquier compilador compatible con C)

## Consideraciones
- El proyecto fue desarrollado colaborativamente en Replit, sin uso de control de versiones (commits).
- Los archivos generados por el programa se almacenan en la carpeta /usuarios.
