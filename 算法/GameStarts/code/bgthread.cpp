#include "bgthread.h"
#include <QUrl>

BGthread::BGthread():effect()
{
    effect.setSource(QUrl::fromLocalFile("music/mySoul.wav"));
    effect.setLoopCount(QSoundEffect::Infinite);
    effect.setVolume(1.0f);
}

void BGthread::run(){
    effect.play();
}

