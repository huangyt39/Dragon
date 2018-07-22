#pragma once

#ifndef DRAGON
#define DRAGON
#include "cocos2d.h"

USING_NS_CC;

#define DRAGONBLUE 1
#define DRAGONBLACK 2

class Dragon
{
public:
	// ���ǵ���ģʽ��Ӧ�ñ����ڳ���������ʵ����������ɫ��0Ϊ�����ƶ���1Ϊ����
	Dragon(int color, Vec2 position);
	
	// ��һ��
	void jump();

	// ����
	void down();

	// ����
	void die();

	// �����ƶ���1Ϊ�ϣ�-1Ϊ��
	void move(int dir);

	// ֹͣ
	void stop();

	// �õ�����
	Sprite *get();

	// ��ʼ��Ϸ�������Ƿ�Ϊ����ģʽ
	void begin(bool gravityEnabled);

	// ���
	void check();

private:
	Sprite * dragon;
	Vector<SpriteFrame*> flying;
};
#endif