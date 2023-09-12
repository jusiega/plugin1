# plugin1
# Paweł Jusięga, Marcin Wójcik
Wtyczka VST3 polegająca na tworzeniu efektu delay typu stereo z modulacją czasu opóźnienia. Niestety modulacja nie może być zbyt duża bo pojawiają się trzaski w odsłuchu. Co ciekawe zauważalność trzasków zależy od wykorzystanego oprogramowania DAW, przykładowo w przypadku programu Reaper są one dużo bardziej zauważalne niż na przykład w programie Ableton. Projekt miał być obrszerniejszy o trzypasmowy equalizer oparty na filrach FIR o współczynnikach wyliczanych z funkcji przenoszenia filtrów analogowych przy pomocy IDFT. Niestety zdaje się że poprawne i kompletne podlinkowanie zewnętrznych bibliotek przy kompilacji jest zadaniem przerastającym studenta fizyki. Niestety przez długi czas łudzono się że jest inaczej. Dowody ku temu załączone są w repozytorium w postaci działającego linuksowego programu wykonującego transformaty Fouriera na wczytanych plikach audio oraz pliki źródłowe wtyczki która miała być equalizerem. Analizując przebieg zdarzeń nie sposób nie zauważyć że w przypadku equalizera praktyczniejszym byłoby jednak zastosować filtry IIR. Kryptyczne wiadomości od kompilatora nie skłoniły jednak do wykonania tego w stosownym czasie. Projekt trudno uznać za skończony ale musi takim się stać.
# Przydatne (konieczne) do kompilacji
System Windows, o ile kompilujący nie jest na tyle odważny aby nie korzystać z poniższej instrukcji,

Program CMake,

Visual Studio C++, jako generator dla CMake,

VST3 SDK: https://github.com/steinbergmedia/vst3sdk

# Kompilacja
W katalogu \vst3sdk\ otworzyc cmake-gui, oznaczyć \vst3sdk\ jako katalog żródłowy, stworzyć weń katalog \build\ i oznaczyć go jako docelowy katalog generacji, dokonać konfiguracji i generacji przy pomocy odpowiedniego generatora, np. "Visual Studio 17 2022". Obok katalogu \vst3sdk\ umieścić katalog \plugin1\. Zmodyfikować plik CMakeLists.txt tak aby zawierał odpowiednią ścieżkę do vst3sdk. Powtórzyć następnie procedurę z cmake-gui, zastępując katalog vst3sdk katalogiem plugin1. Następnie otworzyć terminal (administratora), przenieść się do katalogu \plugin1\build i wywołać komendę:
```
cmake --build . --config Release --target plugin1
```
# Rekomendowane oprogramowanie tesujące
Dowolny DAW, z zastrzeżeniem że niektóre trzeszczą.
# Rekomendowane ustawienia
Byle nie trzeszczało bo wtedy wydaje się że coś jest zepsute. Sprowadza się to do wysokich ustawień "flanger period" oraz niskich ustawień "flanger amplitude"
# Dlaczego trzeszczy?
Modulacja opóźnieniem sygnału wymaga regularnego pomijania jakiejś liczby próbek w odtwarzanym sygnale co wprowadza zniekształcenia. Zastosowano liniową interpolację mającą je wygładzać, jednak na niewiele się ona zdała. Jak zostało wspomniane dużu wpływ może mieć wykorzystany DAW, "host" wtyczki. Podobnie słychać znaczące trzaski w trakcie regulowania wartością opóźnienia.
# Dlaczego w tym piekielnym środowisku i języku
Po odpowiednim zainstancjonowaniu w programie możliwe jest dołączanie efektu do sygnału w czasie zasadniczo rzeczywistym. Umożliwia to przepuszczenie przez nią bez strat na responsywności przykładowo sygnału z instrumentu lub wokalu.
# Gdzie znajduje się wtyczka
Jeśli systemowi powiodło się utworzenie linków symbolicznych do wtyczki to znajdują się one w katalogu %LOCALAPPDATA\Programs\Common\VST3. Jeśli nie, to powinny być w katalogu plugin1\build\VST3\Release. Repozytorium udostępnione jest ze skompilowaną pod system Windows wtyczką.
# Gdzie znajduje się kod stworzony nieautomatyczne
W katalogu plugin1\source. Część jego jednak została wygenerowana automatycznie.
