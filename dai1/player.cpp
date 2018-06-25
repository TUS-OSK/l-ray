#include <cstdio>


class Player {
  public:
    int health; //体力
    int life; //残機
    
    //体力100、残機数3で初期化する
    Player() {
      health = 100;
      life = 3;
    };
    //クラスが破棄されるときに呼ばれる
    ~Player() {}; 
    
    //弾に当たったときにダメージを計算する
    void getDamage() {
      health -= 10; //体力が10減る
      
      //体力が0になったら
      if(health <= 0) {
        life--; //残機が一つ減る
        health = 100; //体力を100に戻しておく
      }
    };

    void print() {
        printf("health:%d life:%d\n", health, life);
    };
};


int main() {
    Player ply = Player();
    ply.getDamage();
    ply.print();
}