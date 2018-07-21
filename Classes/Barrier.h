#pragma once

#ifndef BARRIER
#define BARRIER
#include "cocos2d.h"

USING_NS_CC;

// ��������addChild
class Barrier
{
public:
	// �������ӵ��������Զ�Ϊ��λ)��ģʽ(0,1,2)�����볡��this
	Barrier(int count, int _mode, Layer *scene);

	// ��������Ƿ����
	bool check(Vec2 pos);

	// ��ʼ��Ϸ
	void begin();

	// ��ȡ��������
	std::vector<std::vector<Sprite*>> get();

	// С���λ��
	void move(Vec2 pos, int dir);

	void stop();

	//��Ϸ����ֹͣ����
	void end();
private:
	Sprite * createOne();

	std::vector<std::vector<Sprite*>> all;

	Layer *sc;

	// visibleSize
	Size vs;
	// �Ƿ��Զ������ƶ�
	int mode;
};
#endif