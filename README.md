
English below

A fájl jelenleg a munkámat mutatja be, nem a szoftver használatát, mivel futtatáskor az azonnal lefut., a felhasználónak nincsen további feladata vele.

Magyar

---------- A munkám rövid összefoglalója ----------

A feladatot Visual Studio-ban végeztem C++ nyelven, és az SFML 2.6.1 környezetet használtam a grafikus interface kialakításához.
Ezzel kicsit meg kellett ismerkednem, mivel új volt számomra.

Beolvastam a mellékelt ".csv" fájlt, majd rekordokba szedtem annak tartalmát, az erre a célra létrehozott osztályban.

A grafikus programok természetéből adódóan itt a bal felső sarok az origó és jobbra lefelé van a (+,+) irány a rajzolással,
ezt megváltoztattam, hogy a matematikai tájolással mutassa az adatokat.

A program alapvetően tartalmazna jelmagyarázatot, de ismeretlen okból nem sikerült betölteni betűtípust, hogy kiírhassak bármit is.

A feladatleírással kapcsolatban igyekeztem figyelni a részletekre, az ábrán lévő szenzorok elrendezésére, és a nem egyértelmű adatokra.

Számításaimban a hangsebességet, trigonometrikus azonosságokat használtam főként, továbbá az elmozdulást és további arányokat próbáltam felfedezni a feladatban és a fájlban,
amiből konkrétumokra következtethetnék, de a koordináta sík méretére sajnos nem tudtam rájönni sehogy sem,
így végül egy számomra elfogadható váltószámmal rajzoltam a térképet.

A szenzorok méréséből adódó rajz nem néz ki hibátlanul, sok a kettős fal és érdekes szimmetriákra lettem figyelmes, ami szerintem a jó méretezés és tájolás hiányából adódhat.

Futtatáskor a jármű végighalad a pályán kirajzolva a lemért környezetét, ahogyan azt a fájl definiálja.
A szenzorok helyzetét, és a mérési pontjukat is kiírja különböző színekkel:

US0 - piros
US1 - zöld
US2 - magenta
US3 - sárga

Köztük pedig egy szürkés vonal indikája azok távolságát. A szenzorok csupán 1 pontot mérnek (azaz, nem 30°-os szögben).

A foglaltsági hálót pedig úgy szerettem volna belefoglalni a programba, hogy cellákra osztottam a képet, és ezeken belül számoltam,
hogy hány szenzoros mérés esik egy cellába, majd azt a cellát világosabbra színeztem ezzel szimulálva egy egyszerűsített "Occupancy Grid" modellt,
mely nem tartalmaz komolyabb statisztikai tételeket.
A képnek a háló-felbontása állítható a kódban (res).

Végül igyekeztem minél átláthatóbbá és rendezettebbé tenni a kódot: külön a logikát, külön a rajzolási függvényeket.


English

The file currently demonstrates my work, not how to use the software, since it runs immediately upon execution; the user does not need to do anything else with it.

---------- Short summary of my work ----------

I completed the task in Visual Studio using C++, and I used the SFML 2.6.1 environment to design the graphical interface.
I had to familiarize myself with this a little, as it was new to me.

I read in the attached ".csv" file, and its contents into records in the class created for this purpose.

Due to the nature of graphics programs, the upper left corner is the origin and the (+,+) direction is to the right and down when drawing.
I changed it's orientation to represent the mathematical coordinate system.

The program would normally include a legend, but for some unknown reason, I was unable to load a font to print anything.

Regarding the task description, I tried to pay attention to the details, the arrangement of the sensors in the figure, and the ambiguous data.

In my calculations, I mainly used the speed of sound and trigonometric identities,
and I tried to discover the displacement and other ratios in the task and the file
from which I could draw specific conclusions, but unfortunately I could not figure out the size of the coordinate plane in any way,
so in the end I drew the map with a gear ratio that was acceptable to me.

The drawing resulting from the sensor measurements does not look flawless, there are many double walls and I noticed interesting symmetries,
 which I think may be due to the lack of good scaling and orientation.

When running, the vehicle travels along the track, mapping its measured environment as defined in the file.
It also displays the position of the sensors and their measurement points in different colors:

US0 - red
US1 - green
US2 - magenta
US3 - yellow

A gray line between them indicates their distance. The sensors measure only 1 point (i.e., not at a 30° angle).

I wanted to incorporate the occupancy grid into the program by dividing the image into cells and counting
how many sensor measurements fall into each cell, then coloring that cell lighter to simulate a simplified “Occupancy Grid” model
that does not contain any serious statistical items.
The grid resolution of the image can be adjusted in the code (res).

Finally, I tried to make the code as transparent and organized as possible: separate the logic and the drawing functions.
