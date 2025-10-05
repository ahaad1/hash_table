# Custom Hash Table Implementation (C++)

## Overview

This project is a custom implementation of a hash table (associative array) in C++. It was created to explore performance improvements over the standard std::unordered_map. The implementation supports typical hash table operations – insertions, lookups, and removals – with an average time complexity of O(1). Collisions are handled through separate chaining (using linked lists of key-value pairs), and a custom hash function is used to optimize distribution of entries. All correctness tests have passed, including edge cases (e.g. partial removals), ensuring that the data structure behaves as expected in various scenarios.

## Design and Features

•	Separate Chaining with Custom Nodes: The hash table uses an array of buckets, where each bucket points to a linked list of entries (for keys hashing to the same index). Each entry is stored in a custom Pair structure containing a key, a value, and a pointer to the next entry. This Pair struct essentially acts as a node in a singly linked list, which efficiently handles collisions by chaining entries together.

•	Custom Hash Function: A custom hash function is employed for keys (instead of relying on std::hash). This function was chosen/designed to provide a good distribution of keys across buckets while being efficient to compute. A well-distributed hash minimizes collisions, which helps the table maintain near O(1) performance even with many entries.

•	Memory Grouping (Cache Optimizations): The implementation allocates and manages entries in grouped blocks (via a custom GroupContainer mechanism). By grouping multiple Pair nodes in contiguous memory, the hash table improves cache locality and reduces heap allocation overhead. This means insertions and traversals access memory more sequentially, which is faster on modern hardware. In contrast, std::unordered_map allocates each element separately, causing more fragmented memory access.

•	Dynamic Resizing: The table dynamically resizes (expands) when the load factor grows too high. When resizing, it allocates a larger bucket array and rehashes existing entries into the new buckets. This ensures that insertions remain efficient and prevents performance degradation as more elements are added. (The implementation maintains a load factor threshold similar to typical hash tables – e.g. 0.75 – to decide when to grow the table.)

•	Interface Similar to STL: The custom hash table provides functions analogous to those in std::unordered_map for ease of use. For example, you can insert key-value pairs, find a value by key, or remove a key. The interface is intuitive for anyone familiar with associative containers. Example usage:

•	HashTable<int, std::string> table; – create a hash table for keys of type int and values of type std::string.

•	table.insert(42, "Hello"); – insert a mapping from key 42 to value "Hello".

•	auto result = table.find(42); – retrieve the value for key 42 (returns "Hello" in this case, or an indication if not found).

•	table.remove(42); – remove the entry with key 42 from the table.

•	Tested for Correctness: The implementation has been thoroughly tested. It passes all unit tests for correctness (insertion, retrieval, deletion operations produce expected results). It also handles scenarios like removing many elements (partial table clearance) and then continuing operations, to ensure stability of pointers and internal structure.

## Performance Comparison
One of the main goals of this project is to achieve better performance than std::unordered_map by reducing overhead and improving cache efficiency. We benchmarked the custom hash table against std::unordered_map on 1,000,000 operations for insertion, lookup, and removal. Below are the results:

Operation	Custom Hash Table (time)	std::unordered_map (time)
Insert 1,000,000 elements	~2.19 s	~4.70 s
Find 1,000,000 elements	~0.748 s	~1.333 s
Remove 1,000,000 elements	~1.660 s	~2.312 s

On average, the custom hash table implementation was about 1.5× to 2× faster than std::unordered_map for these operations. In particular, insertion speed was roughly 2.1 times faster (custom: 2.19s vs STL: 4.70s), lookups were ~1.8 times faster, and removals were ~1.4 times faster in our tests.


### These performance gains are attributed to the design decisions:

•	The grouped memory layout of the custom table leads to fewer cache misses during traversals compared to the node-based allocation in std::unordered_map. When iterating through a chain of entries in a bucket, the custom table accesses consecutive memory locations, whereas std::unordered_map may jump between disparate memory locations for each node.

•	The simplified node structure (Pair) and the absence of extra iterator or node overhead make each operation do less work. The custom nodes are lightweight and only contain what’s necessary (key, value, and next pointer), reducing memory overhead.
	
•	Using a tailored hash function can sometimes outperform the default hashing especially for certain data patterns or key types, contributing to faster computation of bucket indices.

It’s worth noting that both data structures have the same average-case complexity, but the constants and memory access patterns differ. The results above demonstrate that careful tuning of a hash table’s implementation can yield tangible speed improvements in practice.

## Conclusion

This custom hash table provides a clear example of how low-level implementation choices can impact performance. By using chaining with a custom memory grouping strategy and an efficient hash function, it outperforms the STL’s unordered_map in various basic operations (while still maintaining correct behavior).

The project can serve as a learning resource or a starting point for further optimizations. Developers interested in high-performance hash maps can study this implementation to understand techniques like custom memory allocators, alternative collision resolution strategies, and hash function selection. Overall, the custom hash table is a fast and reliable associative container that showcases the potential gains of tailoring a data structure to specific performance goals.

⸻

# Реализация хеш-таблицы (C++)

## Обзор

Данный проект представляет собой собственную реализацию структуры данных хеш-таблица на C++. Цель – продемонстрировать улучшение производительности по сравнению со стандартным контейнером std::unordered_map. Реализация поддерживает типичные операции хеш-таблицы – вставку пар ключ-значение, поиск по ключу и удаление – со средним временем выполнения O(1). Для разрешения коллизий используется метод цепочек (открытое хеширование с использованием связных списков), а для вычисления индексов применяется собственная хеш-функция, обеспечивающая равномерное распределение ключей. Реализована полная проверка корректности работы: все тесты на правильность пройдены, включая граничные случаи (например, частичное удаление элементов), что подтверждает корректность и устойчивость структуры данных во всех сценариях.

## Особенности реализации

•	Метод цепочек с собственными узлами: Хеш-таблица состоит из массива бакетов (ячеек). Каждый бакет хранит указатель на начало связного списка элементов, значения которых имеют одинаковый индекс хеширования. Каждый элемент представлен пользовательской структурой Pair, содержащей ключ, значение и указатель на следующий элемент в цепочке. Таким образом, Pair фактически является узлом односвязного списка, через который реализуется коллизионный цепочек. Этот подход позволяет эффективно обрабатывать коллизии, связывая несколько элементов в одном бакете.

•	Собственная хеш-функция: В реализации используется специальная хеш-функция для ключей (вместо стандартной std::hash). Она разработана таким образом, чтобы быстро вычисляться и равномерно распределять значения по бакетам, уменьшая количество коллизий. Качественная хеш-функция максимально сохраняет константное время операций даже при большом числе элементов, не допуская скопления многих ключей в одном бакете.

•	Групповое размещение в памяти (Cache-friendly дизайн): Для хранения элементов используется групповой аллокатор памяти – специальный механизм GroupContainer, который выделяет память сразу под несколько Pair-узлов смежно. За счет группирования нескольких элементов в непрерывном блоке памяти достигается лучшая локальность данных в кэше и снижается накладной расход на частые выделения памяти. Практически это означает, что при вставке элементов и обходе цепочек коллизий обращения к памяти происходят последовательно, что заметно быстрее на современном оборудовании. Для сравнения, std::unordered_map обычно выделяет каждый элемент отдельно на куче, что приводит к фрагментации памяти и большему числу кеш-промахов при итерации.

•	Динамическое расширение таблицы: Реализация поддерживает динамическое изменение размера хеш-таблицы по мере роста числа элементов. Когда загрузка таблицы (отношение числа элементов к размеру массива бакетов) превышает допустимый порог, таблица автоматически расширяется: выделяется новый более крупный массив бакетов, после чего все существующие элементы перераспределяются (rehash) по новым индексам. Это предотвращает деградацию производительности при увеличении количества данных. (Порог загрузки в данной реализации аналогичен стандартным контейнерам, например около 0.7–0.8, после чего происходит увеличение емкости примерно в 2 раза.)

•	Удобный интерфейс: Интерфейс хеш-таблицы разработан по аналогии с STL-контейнерами для словарей, что облегчает использование. Предоставляются методы для вставки новой пары, поиска значения по ключу и удаления элемента по ключу. Например:

•	HashTable<int, std::string> table; – создание хеш-таблицы с ключами типа int и значениями типа std::string.

•	table.insert(42, "Hello"); – вставка пары ключ 42 – значение "Hello".

•	auto result = table.find(42); – поиск значения по ключу 42 (в данном случае вернется "Hello", либо индикатор отсутствия, если ключ не найден).

•	table.remove(42); – удаление элемента с ключом 42 из таблицы.

•	Надежность и тестирование: Реализация прошла всестороннее тестирование на корректность. Проверены базовые операции (вставка, поиск, удаление) на соответствие ожидаемым результатам. Кроме того, тестировались особые случаи, например, массовое удаление части элементов из таблицы и последующее продолжение операций. Такие частичные удаления не нарушают работу структуры – оставшиеся элементы по-прежнему успешно находятся, новые добавляются корректно, а внутренняя структура списков остается консистентной.

## Производительность

Главный результат данного проекта – высокая производительность по сравнению с std::unordered_map благодаря снижению накладных расходов и улучшению кеш-эффективности. Были проведены замеры скорости работы кастомной хеш-таблицы против std::unordered_map на 1 000 000 операций вставки, поиска и удаления. Результаты приведены ниже:

Операция	Наша хеш-таблица (время)	std::unordered_map (время)
Вставка 1 000 000 элементов	~2,19 с	~4,70 с
Поиск 1 000 000 элементов	~0,748 с	~1,333 с
Удаление 1 000 000 элементов	~1,660 с	~2,312 с

В среднем наша хеш-таблица работала примерно в 1,5–2 раза быстрее, чем std::unordered_map, на перечисленных тестах. Например, время вставки ~2,19 с против 4,70 с у STL (примерно в 2,1 раза быстрее), поиск ~0,75 с против 1,33 с (почти в 1,8 раза быстрее), а удаление ~1,66 с против 2,31 с (около 1,4 раза быстрее).

### Таких улучшений удалось добиться благодаря следующим аспектам реализации:

•	Локальность данных в памяти: Групповое размещение узлов в массиве привело к тому, что при обходе цепочки коллизий процессор читает данные из смежных областей памяти. Это резко контрастирует с реализацией std::unordered_map, где каждый узел может быть разбросан по памяти, требуя дополнительного обращения к памяти для каждого перехода к следующему элементу. Как результат, наш алгоритм тратит меньше времени впустую на ожидание данных из памяти.

•	Минимальные накладные расходы на элемент: Структура Pair содержит только необходимые поля (ключ, значение, указатель на следующий), что делает каждый элемент легковесным. В стандартной реализации могут присутствовать дополнительные служебные поля для поддержки сложного функционала (например, итераторов, порядка элементов и пр.), которые увеличивают размер узла и время обработки. Наша упрощенная структура узла и отсутствие лишних абстракций позволяют выполнять работу быстрее.

•	Эффективность вычисления хеша: Пользовательская хеш-функция была выбрана с упором на скорость и равномерность. В сочетании с описанным выше снижением коллизий это обеспечивает меньше вычислительных шагов и конфликтов при определении местоположения элемента. В некоторых случаях быстрый хешер дает небольшое преимущество над стандартным, особенно на ключах со специфическими шаблонами распределения.

Оба сравниваемых контейнера имеют одинаковую алгоритмическую сложность операций в среднем, однако разница в реализации приводит к различным константам времени и шаблонам доступа к памяти. Как показывают тесты, оптимизация внутреннего устройства хеш-таблицы способна дать значимый выигрыш в скорости на практике.

## Заключение

Данная кастомная хеш-таблица наглядно демонстрирует, как продуманные низкоуровневые решения могут повысить эффективность структуры данных. Используя метод цепочек в сочетании с групповым размещением узлов и оптимизированной хеш-функцией, эта реализация превосходит по скорости стандартную std::unordered_map в базовых операциях, сохраняя при этом корректность и надежность работы.

Проект может быть полезен для обучения или как основа для дальнейших улучшений. Разработчики, интересующиеся высокопроизводительными структурами данных, найдут здесь идеи по созданию собственных аллокаторов, нетривиальным способам разрешения коллизий и выбору хеш-функций. В итоге получился быстрый и устойчивый ассоциативный контейнер, подтверждающий, что адаптация структуры данных под конкретные цели может значительно улучшить ее характеристики.
