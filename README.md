# Házi feladat

Név/Nevek NEPTUN kóddal:
- Olexó Gergely (ABRRXW)

# Feladat kiírás
A feladat egy malom játék elkészítése grafikus felülettel. A játék hálózaton keresztül működik két játékos kommunikációjával. A klienseket egy szerverprogram ellenőrzi és felügyeli.
A program az alábbi funkciókkal fog rendelkezni:
* Grafikus felület Qt-ban megvalósítva.
* Helytelen lépés ellenőrzése.
* Játék állapotának követése.
* Játékszabályok instrukcióinak kiírása
* TCP kommunikáció logolása

# Megvalósított program
A grafikus felületet sikerült megvalósítani, azonban kicsit lehetne ergonomikusabb. A socket kommunikáció ilyen egyszerű program esetében jól működik, de ha összetettebb dolgokat is meg kéne valósítani (új játék indítása, játék feladása), akkor más struktúrájú illetve protokollú kommunikációra lenne szükség. A játékot csak úgy lehet újraindítani, ha valamelyik játékos újracsatlakozik a szerverre.

**Bemutató videó URL:**
(Szerepeljen benne minden lényeges funkció és az esetleges eltérések a kiíráshoz képest. max. 5 perc)

# Tapasztalatok
A hálózati kommunikáció logolása és egy kényelmes debug futtatási környezet kialakítása már a program legelején hasznos lehet. Ezt mindenképp hamarabb elvégeztem volna, megspórolva az ezzel járó fennakadást, aminek a végén úgyis el kellett végezni ezeket a dolgokat. A program ergonómiáján is érdemesebb lett volna hamarabb javítani, mert a hibakeresést egyszerűsíti, ha a fejlesztő könnyebben eligazodik a programon.
