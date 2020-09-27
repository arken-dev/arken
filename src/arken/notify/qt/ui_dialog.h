/********************************************************************************
** Form generated from reading UI file 'dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_H
#define UI_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Dialog
{
public:
    QWidget *principal;
    QWidget *conteudo;
    QLabel *notify_label;
    QWidget *topo;
    QHBoxLayout *horizontalLayout;
    QLabel *icon;
    QLabel *label;

    void setupUi(QDialog *Dialog)
    {
        if (Dialog->objectName().isEmpty())
            Dialog->setObjectName(QStringLiteral("Dialog"));
        Dialog->setWindowModality(Qt::WindowModal);
        Dialog->setEnabled(true);
        Dialog->resize(400, 150);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Dialog->sizePolicy().hasHeightForWidth());
        Dialog->setSizePolicy(sizePolicy);
        Dialog->setMinimumSize(QSize(400, 150));
        Dialog->setMaximumSize(QSize(400, 16777215));
        Dialog->setStyleSheet(QLatin1String("gridline-color: rgb(0, 0, 0);\n"
""));
        Dialog->setModal(false);
        principal = new QWidget(Dialog);
        principal->setObjectName(QStringLiteral("principal"));
        principal->setGeometry(QRect(0, 0, 400, 150));
        sizePolicy.setHeightForWidth(principal->sizePolicy().hasHeightForWidth());
        principal->setSizePolicy(sizePolicy);
        principal->setMinimumSize(QSize(150, 0));
        principal->setStyleSheet(QStringLiteral(""));
        conteudo = new QWidget(principal);
        conteudo->setObjectName(QStringLiteral("conteudo"));
        conteudo->setGeometry(QRect(0, -1, 400, 150));
        conteudo->setMinimumSize(QSize(150, 0));
        conteudo->setStyleSheet(QStringLiteral("background-color: #fbfbfb;"));
        notify_label = new QLabel(conteudo);
        notify_label->setObjectName(QStringLiteral("notify_label"));
        notify_label->setGeometry(QRect(10, 50, 380, 85));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(1);
        sizePolicy1.setHeightForWidth(notify_label->sizePolicy().hasHeightForWidth());
        notify_label->setSizePolicy(sizePolicy1);
        notify_label->setMinimumSize(QSize(380, 0));
        notify_label->setMaximumSize(QSize(380, 16777215));
        notify_label->setSizeIncrement(QSize(0, 1));
        QFont font;
        font.setFamily(QStringLiteral("Verdana"));
        font.setPointSize(8);
        font.setBold(false);
        font.setWeight(50);
        font.setStyleStrategy(QFont::PreferAntialias);
        notify_label->setFont(font);
        notify_label->setCursor(QCursor(Qt::ArrowCursor));
        notify_label->setStyleSheet(QStringLiteral("color: rgb(21, 21, 21);"));
        notify_label->setInputMethodHints(Qt::ImhMultiLine);
        notify_label->setTextFormat(Qt::AutoText);
        notify_label->setScaledContents(true);
        notify_label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        notify_label->setWordWrap(true);
        notify_label->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
        topo = new QWidget(conteudo);
        topo->setObjectName(QStringLiteral("topo"));
        topo->setGeometry(QRect(0, 0, 400, 34));
        topo->setMinimumSize(QSize(400, 34));
        topo->setMaximumSize(QSize(400, 34));
        topo->setStyleSheet(QStringLiteral("background-color: #546e7a;"));
        horizontalLayout = new QHBoxLayout(topo);
        horizontalLayout->setSpacing(10);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        horizontalLayout->setContentsMargins(10, 0, 0, 0);
        icon = new QLabel(topo);
        icon->setObjectName(QStringLiteral("icon"));
        icon->setMinimumSize(QSize(20, 20));
        icon->setMaximumSize(QSize(20, 20));

        horizontalLayout->addWidget(icon);

        label = new QLabel(topo);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(380, 34));
        label->setMaximumSize(QSize(380, 34));
        QFont font1;
        font1.setFamily(QStringLiteral("Verdana"));
        font1.setPointSize(10);
        font1.setBold(true);
        font1.setWeight(75);
        font1.setStyleStrategy(QFont::PreferAntialias);
        label->setFont(font1);
        label->setStyleSheet(QStringLiteral("color: rgb(255, 255, 255);"));
        label->setTextInteractionFlags(Qt::LinksAccessibleByMouse);

        horizontalLayout->addWidget(label);


        retranslateUi(Dialog);

        QMetaObject::connectSlotsByName(Dialog);
    } // setupUi

    void retranslateUi(QDialog *Dialog)
    {
        Dialog->setWindowTitle(QApplication::translate("Dialog", "Dialog", Q_NULLPTR));
        notify_label->setText(QApplication::translate("Dialog", "texto", Q_NULLPTR));
        icon->setText(QString());
        label->setText(QApplication::translate("Dialog", "Aten\303\247\303\243o", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Dialog: public Ui_Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_H
