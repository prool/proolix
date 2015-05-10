proolix
=======

Simple POSIX-compatible OS for i8086

---

Proolix состоит из двух проектов:

Proolix-d - хронологически первый, средой разработки является проприетарная среда MSDOS/MS Windows и проприетарный компилятор Turbo C 2.0/tasm.
(Хотя он уже вроде как стал бесплатным, но не свободным)

Проект Д загружался и даже как-то работал!

Proolix-l - это рефакторинг проекта Д, уже в среде Linux с использованием компиляторов gcc/as. Там разработан только бутсектор и простейшее ядро. Цель рефакторинга: переход на исключительно свободное ПО

Недавно, имея на рабочей машине только Ubuntu Linux я думал, как собрать проект Д. В виртуальной машине? И вот я сделал проще - собрал все в DOSbox. И оно собралось, правда ранее работающее ядро сразу виснет. Надо разбираться, что я делал не так, может какие параметры компиляции или версии tc/tasm/pklite (ядро у меня потом упаковывается при помощи pklite и представляет собой файл формата exe, а в boot manager встроен простой загрузчик exe файлов. На это пришлось пойти, так как по мере роста объема кода ядро уже не умещалось в модель памяти tiny (то есть в один сегмент 64К. Напоминаю, что ОС Proolix работает в реальном режиме процессора i8086 и совместимых с ним)

---


Russian comments in sources

- in oldest files in CP866 ("MSDOS alternative") codetable
- in old files in KOI8-R codetable
- in new files in UTF-8 codetable

Prool

---

-- 
Proolix files here http://prool.kharkov.org/proolix/

Proolix maillist http://subscribe.ru/catalog/comp.soft.othos.proolix/

Prool page http://prool.kharkov.org
