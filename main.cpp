#include "QtWidgetsApplication1.h"
#include <QTimer>
#include <QtWidgets/QApplication>
#include <thread>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QtWidgetsApplication1 window;
    window.show();
	//faire en boucle la fonction tempLoop
	std::thread loopThread([&window]() {
		while (true) {
			tempLoop(&window);
			std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		}
		});
	loopThread.detach();
	QTimer::singleShot(300000, &app, &QApplication::quit);
    return app.exec();
}