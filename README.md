# АВС-ИДЗ4. Крупнов Иван, БПИ217, вариант 20

**Индивидуальное домашнее задание №4 по курсу Архитектуры вычислительных систем. Домашнее задание выполнено на предполагаемую оценку null.**

## Задание
**Задача о программистах.** В отделе работают три программиста. Каждый программист пишет свою программу и отдает ее на проверку другому случайному программисту. Программист проверяет чужую программу, когда его собственная уже написана и передана на проверку. По завершении проверки, программист возвращает программу с результатом (формируемым случайно по любому из выбранных Вами законов): программа написана правильно или неправильно. Программист спит, если отправил свою программу и не проверяет чужую программу. Программист просыпается, когда получает заключение от другого программиста. Если программа признана правильной, программист пишет другую программу, если программа признана неправильной, программист исправляет ее и отправляет на проверку тому же программисту, который ее проверял. К исправлению своей программы он приступает, завершив проверку чужой программы. При наличии в очереди проверяемых программ и приходе заключения о неправильной своей программы программист может выбирать любую из возможных работ. Создать многопоточное приложение, моделирующее работу программистов.
