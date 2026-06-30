#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QProcess>
#include "time_loop.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWidget main_menu;
    main_menu.resize(1000, 1000);
    main_menu.setStyleSheet("background-color: rgb(128,128,128);");
    QLabel title("Time L\u221E p!", &main_menu);
    title.setGeometry(0, 10, 1000, 200);
    title.move(80,-40);
    QFont font = title.font();
    font.setBold(true);
    font.setUnderline(true);
    font.setPointSize(50); 
    title.setFont(font);
    title.setStyleSheet("Color: red;");
    title.setAlignment(Qt::AlignCenter);
    QPushButton start_game("Start",&main_menu);
    start_game.setGeometry(520,120,120, 60);
    start_game.setStyleSheet("background-color: #000000;" "color: blue;" "font-size: 25px;" "padding: 10px;");
    QPushButton info("Info",&main_menu);
    info.setGeometry(520,220,120, 60);
    info.setStyleSheet("background-color: #000000;" "color: blue;" "font-size: 25px;" "padding: 10px;");
    QPushButton quit_game("Quit",&main_menu);
    quit_game.setGeometry(520,320,120, 60);
    quit_game.setStyleSheet("background-color: #000000;" "color: blue;" "font-size: 25px;" "padding: 10px;");

    QLabel infotext(&main_menu);
    infotext.setText("Welcome to Time Loop! You are a mad scientist tring to leave the lab to go home. The exit is behind a door that can be opened by pressing a button. However you are alone. How will you escape? Arrow keys to move, space bar to lop back in time!");
    infotext.setGeometry(250,420,500,300);
    infotext.setStyleSheet("color: black; font-size: 25px;");
    infotext.setAlignment(Qt::AlignCenter);
    infotext.setWordWrap(true);
    infotext.hide();

    QObject::connect(&info,&QPushButton::clicked, [&](){
      infotext.setVisible(!infotext.isVisible());
    });

    QObject::connect(&quit_game,&QPushButton::clicked, [](){
      QCoreApplication::quit();
    });

     QObject::connect(&start_game, &QPushButton::clicked, [&main_menu](){
    main_menu.hide();
    run_game();   
    main_menu.show();
});
    
    main_menu.show();
    main_menu.setWindowTitle(QApplication::translate("toplevel", "Time Loop!"));
    return app.exec();
}
