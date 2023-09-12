# plugin1
Wtyczka VST3 polegająca na tworzeniu efektu delay typu stereo z modulacją czasu opóźnienia. Niestety modulacja nie może być zbyt duża bo pojawiają się trzaski w odsłuchu. Co ciekawe zauważalność trzasków zależy od wykorzystanego oprogramowania DAW, przykładowo w przypadku programu Reaper są one dużo bardziej zauważalne niż na przykład w programie Ableton. Projekt miał być obrszerniejszy o trzypasmowy equalizer oparty na filrach FIR o współczynnikach wyliczanych z funkcji przenoszenia filtrów analogowych przy pomocy IDFT. Niestety zdaje się że poprawne i kompletne podlinkowanie zewnętrznych bibliotek przy kompilacji jest zadaniem przerastającym studenta fizyki. Niestety przez długi czas łudzono się że jest inaczej. Dowody ku temu załączone są w repozytorium w postaci działającego linuksowego programu wykonującego transformaty Fouriera na wczytanych plikach audio oraz pliki źródłowe wtyczki która miała być equalizerem. Analizując przebieg zdarzeń nie sposób nie zauważyć że w przypadku equalizera praktyczniejszym byłoby jednak zastosować filtry IIR. Kryptyczne wiadomości od kompilatora nie skłoniły jednak do wykonania tego w stosownym czasie. Projekt trudno uznać za skończony ale musi takim się stać.
# Przydatne (konieczne) do kompilacji
System Windows, o ile kompilujący nie jest na tyle odważny aby nie korzystać z poniższej instrukcji,

Program CMake,

Visual Studio C++, jako generator dla CMake,

VST3 SDK: https://github.com/steinbergmedia/vst3sdk

#Kompilacja
W katalogu \vst3sdk\ należy przy pomocy programu PowerShell wywołać CMake
```
cmake -G "Visual Studio 17 2022" -A x64 .
