# Java Mathx Library

**EN:** This library provides a set of mathematical functions implemented in **C++ native** code using **JNI (Java Native Interface)**. It focuses on custom implementations of algorithms like Taylor series and Newton-Raphson for high-performance calculations.

**TR:** Bu kütüphane, **JNI (Java Native Interface)** aracılığıyla **C++ native** kod kullanılarak geliştirilmiş matematiksel fonksiyonlar sunar. Taylor serisi ve Newton-Raphson gibi algoritmaların performanslı özel uygulamalarına odaklanır.

---

## Derleme / Compilation

```bash
# C++ (Linux)
g++ -shared -fPIC -I"$JAVA_HOME/include" -I"$JAVA_HOME/include/linux" Mathx.cpp -o libmathx.so

# Java
javac Mathx.java
javac App.java
java -Djava.library.path=. App
