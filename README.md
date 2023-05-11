## TopWords

This is a simple Qt-based application which allows to analyze a textual file in order to show the 15 top most frequent words in it.

File reading is happening in the worker thread in order to do not block the UI. Communication between main (gui) and worker threads is implemented in a Request–Response manner using Qt signals/slots. Such approach allows to not use such syncronisation primitives as mutexes and semaphores. All syncronisation logic between threads in this case falls on Qt engine's shoulders.

In general, the application follows Model-View-Controller design pattern (MVC). The Controller owns and binds together business logic with data and exports it to Qml in a form of Q_PROPERTYs and Q_INVOKABLE methods.
​

## Prerequisites

- Minimal Qt version is 5.15
- Compiler with C++11 support
​

## Build & Run

- Use QtCreator to build the project (project file - TopWords.pro)
