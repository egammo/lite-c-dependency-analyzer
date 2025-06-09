# Code-Abhängigkeits-Analyzer - Benutzeranleitung

## Was ist der Code-Abhängigkeits-Analyzer?

Der Enhanced C/C++ Code Dependency Analyzer ist ein Werkzeug, das Ihren Lite-C/C++ Code analysiert und Ihnen zeigt, wie Ihre Dateien miteinander verbunden sind. Er findet Probleme wie zirkuläre Abhängigkeiten und erstellt einen detaillierten Bericht über Ihre Code-Struktur.

## Verwendung

### Grundlegende Befehle

**Include-Verfolgung (empfohlen):**
```bash
analyzer.exe .\mein_projekt main.c analyse.txt
```

**Alle Dateien scannen:**
```bash
analyzer.exe .\mein_projekt "" vollanalyse.txt
```

## Die Ausgabedatei verstehen

### 1. Header-Informationen

```
=== ENHANCED C/C++ CODE DEPENDENCY ANALYSIS v1.3 FIXED ===
Generated: 21:25:33

Analysis Mode: Include Tracking
Total Files Found: 178
Actually Used Files: 178
Global PRAGMA_PATH entries found: 24
Files with Circular Includes: 178
Files with Performance Problems: 0
```

**Was bedeutet das?**
- **Analysis Mode:** Zeigt an, ob von einer Hauptdatei aus gefolgt wurde oder alle Dateien gescannt wurden
- **Total Files Found:** Anzahl aller gefundenen .c/.h Dateien
- **Actually Used Files:** Davon tatsächlich verwendete Dateien
- **Global PRAGMA_PATH entries:** Anzahl der Pfaddefinitionen
- **Circular Includes:** Dateien mit zirkulären Abhängigkeiten (⚠️ Problem!)
- **Performance Problems:** Dateien mit zu tiefen Verschachtelungen

### 2. Warnungen verstehen

#### Zirkuläre Include-Warnungen
```
=== CIRCULAR INCLUDE WARNINGS ===
WARNING: main.c (depth 0) - Part of circular include chain
WARNING: cmd.h (depth 1) - Part of circular include chain
```

**Was bedeutet das?**
- ⚠️ **Circular Include:** Dateien, die sich gegenseitig einbinden
- **Depth:** Verschachtelungstiefe der Include-Kette
- **Problem:** Kann zu endlosen Schleifen beim Kompilieren führen

#### PRAGMA_PATH Definitionen
```
=== PRAGMA_PATH DEFINITIONS ===
0042  #define PRAGMA_PATH "Src" (in main.c)
0043  #define PRAGMA_PATH "Src\ui" (in main.c) [BACKSLASH]
```

**Was bedeutet das?**
- **PRAGMA_PATH:** Verzeichnisse, in denen nach Include-Dateien gesucht wird
- **[BACKSLASH]:** Warnung - Backslashes können Probleme verursachen
- **Zeilennummer:** Wo im Code diese Definition steht

### 3. Detaillierte Datei-Analyse

#### Datei-Header
```
=== main.c ===
Directory: .\main.c
Include Depth: 0 [CIRCULAR INCLUDE DETECTED]
Status: USED
Multiple Includes: 2 times
```

**Was bedeutet das?**
- **Directory:** Wo die Datei liegt
- **Include Depth:** Wie tief verschachtelt (0 = Hauptdatei)
- **Status:** USED = wird verwendet, UNUSED = nicht verwendet
- **Multiple Includes:** Wird mehrfach eingebunden

#### Include-Liste
```
INCLUDES:
0081  #include "cmd.h"
0082  #include "winter_fog.h" is "winter_fog.h"
0086  #include "Shaders\ppSsao.h" [BACKSLASH]
```

**Was bedeutet das?**
- **Zeilennummer:** Wo der Include steht
- **Include-Name:** Welche Datei eingebunden wird
- **"is":** Zeigt den aufgelösten Pfad
- **[BACKSLASH]:** Warnung für Pfad-Probleme

#### Code-Elemente

**Void Functions (Funktionen ohne Rückgabewert):**
```
VOID FUNCTIONS:
0074  void um_destroy()
0075  void um_open()
```

**Functions (Lite-C Funktionen):**
```
FUNCTIONS:
0600  function objMtlDeferredSwitch_ev()
0610  function mtlSsaoParticle_ev()
```

**Actions (Lite-C Aktionen):**
```
ACTIONS:
0089  action enemy_dynamic_std()
0109  action enemy_dynamic_dumb()
```

**Structs (Datenstrukturen):**
```
TYPEDEF STRUCTS:
0568  typedef struct SsaoObjMtl
0614  typedef struct technique t
```

**Static Variables (Statische Variablen):**
```
STATIC VARIABLES:
1016  static pre (global)
0187  static grpIdx (in function: LfpAddLight)
```

### 4. Zusammenfassung

```
=== SUMMARY ===
Total Files Analyzed: 178
Files with Circular Includes: 178
Files with Performance Problems: 0
Global PRAGMA_PATH entries: 24
```

## Häufige Probleme und Lösungen

### 🔴 Problem: Zirkuläre Includes
**Symptom:** `Files with Circular Includes: X` ist größer als 0

**Was passiert:**
- Datei A bindet Datei B ein
- Datei B bindet Datei A ein
- Endlose Schleife beim Kompilieren

**Lösung:**
1. Forward-Deklarationen verwenden
2. Gemeinsame Header-Datei erstellen
3. Include-Struktur umgestalten

### 🟡 Problem: Performance-Warnungen
**Symptom:** `Files with Performance Problems: X` ist größer als 0

**Was passiert:**
- Include-Ketten sind zu tief verschachtelt (>10 Ebenen)
- Kompilierzeit wird sehr lang

**Lösung:**
1. Weniger Include-Ebenen verwenden
2. Direkte Includes bevorzugen
3. Überflüssige Includes entfernen

### 🟡 Problem: Backslash-Warnungen
**Symptom:** `[BACKSLASH]` Markierungen in der Ausgabe

**Was passiert:**
- Pfade verwenden Backslashes (\) statt Forward-Slashes (/)
- Kann Probleme auf anderen Betriebssystemen verursachen

**Lösung:**
- Forward-Slashes (/) verwenden: `#include "src/header.h"`
- Oder PRAGMA_PATH ohne Backslashes definieren

### 🔵 Problem: Nicht gefundene Includes
**Symptom:** `"NOT FOUND"` in der Include-Liste

**Was passiert:**
- Include-Datei kann nicht gefunden werden
- Falsche Pfade oder fehlende PRAGMA_PATH

**Lösung:**
1. Pfade überprüfen
2. PRAGMA_PATH korrekt definieren
3. Dateien an die richtige Stelle kopieren

## Tipps für sauberen Code

### ✅ Gute Praktiken
- **Wenige Include-Ebenen:** Maximal 5-6 Ebenen
- **Forward-Deklarationen:** Reduziert Abhängigkeiten
- **Klare Ordnerstruktur:** Logische Aufteilung
- **Forward-Slashes:** Für bessere Kompatibilität

### ❌ Vermeiden Sie
- **Zirkuläre Includes:** Datei A → Datei B → Datei A
- **Tiefe Verschachtelungen:** Mehr als 10 Include-Ebenen
- **Backslashes in Pfaden:** Verwenden Sie `/` statt `\`
- **Überflüssige Includes:** Nur notwendige Dateien einbinden

## Beispiel-Workflow

1. **Analyzer ausführen:**
   ```bash
   analyzer.exe .\mein_projekt main.c analyse.txt
   ```

2. **Bericht prüfen:**
   - Circular Includes → 🔴 Sofort beheben
   - Performance Problems → 🟡 Bei Gelegenheit beheben
   - Backslash-Warnungen → 🟡 Bei Gelegenheit beheben

3. **Code verbessern:**
   - Zirkuläre Abhängigkeiten auflösen
   - Überflüssige Includes entfernen
   - Pfade vereinheitlichen

4. **Erneut testen:**
   ```bash
   analyzer.exe .\mein_projekt main.c neue_analyse.txt
   ```

## Ausgabeformat-Referenz

| Abschnitt | Beschreibung |
|-----------|--------------|
| `CIRCULAR INCLUDE WARNINGS` | ⚠️ Dateien mit zirkulären Abhängigkeiten |
| `PRAGMA_PATH DEFINITIONS` | 📁 Pfaddefinitionen für Include-Suche |
| `INCLUDES` | 📄 Liste aller eingebundenen Dateien |
| `VOID FUNCTIONS` | ⚙️ Funktionen ohne Rückgabewert |
| `FUNCTIONS` | ⚙️ Lite-C function-Deklarationen |
| `ACTIONS` | 🎬 Lite-C action-Deklarationen |
| `TYPEDEF STRUCTS` | 🏗️ Datenstruktur-Definitionen |
| `STATIC VARIABLES` | 📊 Statische Variablen |

## Weitere Hilfe

Bei Problemen oder Fragen:
1. Prüfen Sie die Warnungen in der Ausgabedatei
2. Folgen Sie den Lösungsvorschlägen oben
3. Konsultieren Sie die Lite-C Dokumentation
4. Überprüfen Sie Ihre Include-Pfade und PRAGMA_PATH Definitionen

Der Analyzer hilft Ihnen dabei, Ihren Code sauberer und wartbarer zu machen!