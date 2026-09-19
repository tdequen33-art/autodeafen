#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <fstream>

using namespace geode::prelude;

static int dernier = -1;

static void ecrire(int valeur) {
    static bool dejaLog = false;
    auto chemin = Mod::get()->getSaveDir() / "pourcentage.txt";
    std::ofstream f(chemin);
    if (!f) {
        if (!dejaLog) {
            log::error("AutoDeafen : impossible d'ouvrir {}", chemin.string());
            dejaLog = true;
        }
        return;
    }
    f << valeur;
    if (!dejaLog) {
        log::info("AutoDeafen : ecriture OK dans {}", chemin.string());
        dejaLog = true;
    }
}

class $modify(PlayLayer) {
    void updateProgressbar() {
        PlayLayer::updateProgressbar();
        int p = (int)this->getCurrentPercent();
        if (p != dernier) {
            dernier = p;
            ecrire(p);
        }
    }

    void resetLevel() {
        PlayLayer::resetLevel();
        dernier = 0;
        ecrire(0);
    }

    void levelComplete() {
        PlayLayer::levelComplete();
        dernier = 0;
        ecrire(0);
    }

    void onQuit() {
        dernier = 0;
        ecrire(0);
        PlayLayer::onQuit();
    }
};
