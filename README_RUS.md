# Maze

Реализация проекта Maze.
Разработка на языке программирования C++, с использованием паттерна MVC, графический пользовательский интерфейс на базе QT-библиотеки, программы позволяющая генерировать и отрисовывать идеальные лабиринты(согласно алгоритму Эллера, без изолированных областей и петель, с поиском пути в между двумя точками в любом месте лабиринта), и пещеры(с использованием клеточного автомата, пользователем задаются пределы "рождения" и "смерти" клетки, а также шанс на начальную инициализацию клетки).

## Information

Лабиринт с "тонкими стенками" представляет собой таблицу размером _n_ строк на _m_ столбцов. 
Между ячейками таблицы могут находиться "стены". Также "стенами" окружена вся таблица в целом. 

Далее приведён пример такого лабиринта: \
![maze](misc/images/maze.jpg)

Решением лабиринта считается кратчайший путь от заданной начальной точки (ячейки таблицы) до конечной. 
При прохождении лабиринта можно передвигаться к соседним ячейкам, не отделенным "стеной" от текущей ячейки и находящимся сверху, снизу, справа или слева. 
Кратчайшим маршрут считается, если он проходит через наименьшее число ячеек.

Пример лабиринта с его решением: \
![solution](misc/images/solution.jpg)

В этом примере начальная точка задана, как 10; 1, а конечная, как 6; 10.

## Описание лабиринта

Лабиринт может храниться в файле в виде количества строк и столбцов, а также двух матриц, содержащих положение вертикальных и горизонтальных стен соответственно. 
В первой матрице отображается наличие стены справа от каждой ячейки, а во второй - снизу. 

Пример подобного файла:  
```
4 4
0 0 0 1
1 0 1 1
0 1 0 1
0 0 0 1

1 0 1 0
0 0 1 0
1 1 0 1
1 1 1 1
```

Лабиринт, описанный в этом файле: \
![maze4](misc/images/maze4.jpg)

Больше примеров описания лабиринтов можно найти в материалах.

## Недостатки лабиринтов

К недостаткам лабиринтов относятся изолированные области и петли.

Изолированная область - это часть лабиринта с проходами, в которые нельзя попасть из оставшейся части лабиринта. Например: \
![isolated](misc/images/isolated.png)

Петля - это часть лабиринта с проходами, по которым можно ходить "кругами". Стены в петлях не соединены со стенами, окружающими лабиринт. Например: \
![loop](misc/images/loop.png)

## Генерация с использованием клеточного автомата

Во многих играх есть необходимость в ветвящихся локациях, например пещерах. 
Такие локации могут быть созданы генерацией с использованием клеточного автомата. 
При подобной генерации используется идея, схожая с уже знакомой вам игрой "Жизнь". 
Суть предложенного алгоритма состоит в реализации всего двух шагов: 
сначала все поле заполняется случайным образом стенами — т.е. для каждой клетки случайным образом определяется, 
будет ли она свободной или непроходимой — а затем несколько раз происходит обновление состояния карты в соответствии с условиями, 
похожими на условия рождения/смерти в «Жизни».

Правила проще, чем в "Жизни" - есть две специальные переменные, одна для "рождения" "мертвых" клеток (предел "рождения") и одна для уничтожения "живых" клеток (предел "смерти"). 
Если "живые" клетки окружены "живыми" клетками, количество которых меньше, чем предел "смерти", они "умирают". 
Аналогично если "мертвые" клетки находятся рядом с "живыми", количество которых больше, чем предел "рождения", они становятся "живыми".

Пример результата работы алгоритма (на первой картинке только инициализированный лабиринт, а на второй лабиринт, в котором при последующих шагах больше не происходит изменений: \
![cave1](misc/images/cave1.jpg)
![cave2](misc/images/cave2.jpg)

## Описание пещер

Пещера, прошедшая 0 шагов симуляции (только инициализированная), может храниться в файле в виде количества строк и столбцов, 
а также матрицы, содержащей положение "живых" и "мертвых" клеток.

Пример подобного файла:
```
4 4
0 1 0 1
1 0 0 1
0 1 0 0
0 0 1 1
```

Пещера, описанная в этом файле: \
![cave3](misc/images/cave3.jpg)

Больше примеров описания пещер можно найти в материалах.

## Part 1. Реализация проекта Maze

Реализация программу Maze позволяет генерировать и отрисовывать идеальные лабиринты и пещеры:
- В программе быть реализован графический пользовательский интерфейс на базе GUI-библиотеки Qt
- В программе предусмотрена кнопка для загрузки лабиринта из файла, который задается в формате, описанном [выше](#описание-лабиринта) 
- Максимальный размер лабиринта - 50х50
- Загруженный лабиринт будет отрисован на экране в поле размером 500 x 500 пикселей
- Толщина "стены" - 2 пикселя
- Размер самих ячеек лабиринта вычисляется таким образом, чтобы лабиринт занимал всё отведенное под него поле

## Part 2. Генерация идеального лабиринта

Есть возможность автоматической генерации идеального лабиринта. \
Идеальным считается лабиринт, в котором из каждой точки можно попасть в любую другую точку ровно одним способом.

- Генерировать лабиринт нужно согласно **алгоритму Эллера**
- Сгенерированный лабиринт не должен иметь изолированных областей и петель
- Должно быть обеспечено полное покрытие unit-тестами модуля генерации идеального лабиринта
- Пользователем вводится только размерность лабиринта: количество строк и столбцов
- Сгенерированный лабиринт сохраняется в файл в формате, описанном [выше](#описание-лабиринта) 
- Созданный лабиринт отображается на экране как указано в [первой части](#part-1-реализация-проекта-maze)

## Part 3. Решение лабиринта

Есть возможность показать решение _любого_ лабиринта, который сейчас изображен на экране:
- Пользователем задаются начальная и конечная точки
- Маршрут, являющийся решением, отобразить линией толщиной 2 пикселя, проходящей через середины всех ячеек лабиринта, через которые пролегает решение
- Цвет линии решения будет отличным от цветов стен и поля
- Обеспечено полное покрытие unit-тестами модуля решения лабиринта

## Part 4. Генерация пещер

Генерация пещер с [использованием клеточного автомата](#генерация-с-использованием-клеточного-автомата):
- Пользователем выбирается файл, в котором описан пещера по описанному [выше](#описание-пещер) формату
- Для отображения пещер использовать отдельное окно или вкладку пользовательского интерфейса
- Максимальный размер пещеры - 50 х 50
- Загруженная пещера должна быть отрисована на экране в поле размером 500 x 500 пикселей
- Пользователем задаются пределы "рождения" и "смерти" клетки, а также шанс на начальную инициализацию клетки
- Пределы "рождения" и "смерти" могут иметь значения от 0 до 7
- Предусмотрен пошаговый режим отрисовки результатов работы алгоритма в двух вариантах:
  - По нажатию на кнопку следующего шага отрисовывается очередная итерация работы алгоритма
  - По нажатию на кнопку автоматической работы запускается отрисовка итераций работы алгоритма с частотой 1 шаг в `N` миллисекунд, где число миллисекунд `N` задаётся через специальное поле в пользовательском интерфейсе
- Размер клеток в пикселях вычисляется таким образом, чтобы пещера занимала всё отведенное под него поле
- Обеспечено полное покрытие unit-тестами модуля генерации пещер
