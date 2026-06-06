#include "coin.h"
#include <QBrush>
#include <QColor>
#include <QRandomGenerator>

Coin::Coin(QGraphicsItem *parent)
{
    // 生成 0 到 99 的随机数，用于 4:3:3 的概率分配
    int randVal = QRandomGenerator::global()->bounded(100);
    QString imagePath;

    if (randVal < 40) {
        m_coinType = GPT;
        imagePath = ":/new/prefix2/coin_GPT.png";        // GPT 图片
    } else if (randVal < 70) {
        m_coinType = MATH_BOOK;
        imagePath = ":/new/prefix2/coin_book.png";   // 高数书图片
    } else {
        m_coinType = EMAIL;
        imagePath = ":/new/prefix2/coin_email.png";      // 邮件图片
    }

    QPixmap pix(imagePath);
    if (!pix.isNull()) {
        setPixmap(pix.scaled(40, 40, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }
}



