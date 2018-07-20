#include "Barrier.h"

Barrier::Barrier(int count, int _mode, Layer *scene):  mode(_mode), sc(scene)
{
	vs = Director::getInstance()->getVisibleSize();
	all.reserve(count);
	for (int i = 0; i < count; ++i) {
		auto up = createOne();
		Vec2 pos(vs.width + i * 300,
			random(200.0f,  vs.height) + up->getContentSize().height / 2);
		up->setPosition(pos);
		up->setRotation(180);
		sc->addChild(up);

		auto down = createOne();
		down->setPosition(Vec2(pos.x, pos.y - 200));
		sc->addChild(down);

		std::vector<Sprite *>toAdd{ up, down };
		all.push_back(toAdd);
	}
}

void Barrier::check(Vec2 pos)
{
	// 检查是否走出边界
	int end = all.size();
	for (int i = 0; i < end; ++i) {
		auto pos = all.at(i)[0]->getPosition();
		if (pos.x < -45) {
			auto newUp = createOne();
			auto newDown = createOne();
			newUp->setPosition(Vec2(all.back()[0]->getPositionX() + 300, pos.y));
			newDown->setPosition(Vec2(all.back()[0]->getPositionX() + 300, pos.y - 200));
			if (mode == 1) {
				auto v = all.at(i)[0]->getPhysicsBody()->getVelocity();
				newUp->getPhysicsBody()->setVelocity(v);
				newDown->getPhysicsBody()->setVelocity(v);
			}
			all.erase(std::next(all.begin(), i));
			std::vector<Sprite*> toAdd{ newUp, newDown };
			all.push_back(toAdd);
			--i;
			--end;
		}
	}

	// 检查是否应该调头
	for (int i = 0; i < all.size(); ++i) {
		auto up = all.at(i)[0];
		auto down = all.at(i)[1];
		if (up->getBoundingBox().getMinY() > vs.height - 50 || down->getBoundingBox().getMaxY() < 50) {
			auto v = up->getPhysicsBody()->getVelocity();
			up->getPhysicsBody()->setVelocity(Vec2(v.x, mode == 1 ? -v.y : 0));
			down->getPhysicsBody()->setVelocity(Vec2(v.x, mode == 1 ? -v.y : 0));
		}
	}

	if (mode != 2) return;
	// 判断是否控制权应该切换到下一个柱子
	for (int i = 0; i < all.size(); ++i) {
		auto one = all.at(i)[0];
		auto v = one->getPhysicsBody()->getVelocity();
		if (v.y != 0 && one->getBoundingBox().getMinX() < pos.x) {
			auto next = all.at(i + 1);
			next[0]->getPhysicsBody()->setVelocity(Vec2(v.x, v.y));
			next[1]->getPhysicsBody()->setVelocity(Vec2(v.x, v.y));
		}
	}
}

void Barrier::begin()
{
	int dir = mode == 1 ? 1: 0, count = 0;
	for (auto one : all) {
		one[0]->getPhysicsBody()->setVelocity(Vec2(-100, 100 * dir));
		one[1]->getPhysicsBody()->setVelocity(Vec2(-100, 100 * dir));
		++count;
		if (count == 2) {
			count = 0;
			dir = -dir;
		}
	}
}

Sprite* Barrier::createOne()
{
	auto one = Sprite::create("stone.png");
	auto body = PhysicsBody::createBox(Size(420, 90), PhysicsMaterial(100.0f, 0.0f, 1.0f));
	body->setContactTestBitmask(0xFFFFFFFF);
	body->setDynamic(false);
	one->setPhysicsBody(body);
	one->setTag(1);
	return one;
}

std::vector<std::vector<Sprite*>> Barrier::get() {
	return all;
}

void Barrier::move(Vec2 pos, int dir)
{
	for (int i = 0; i < all.size(); ++i) {
		auto up = all.at(i)[0];
		auto down = all.at(i)[1];
		if (up->getBoundingBox().getMinX() > pos.x) {
			up->getPhysicsBody()->setVelocity(Vec2(-100, 100 * dir));
			down->getPhysicsBody()->setVelocity(Vec2(-100, 100 * dir));
			break;
		}
	}
}

void Barrier::stop()
{
	for (int i = 0; i < all.size(); ++i) {
		auto v = all.at(i)[0]->getPhysicsBody()->getVelocity();
		if (v.y != 0) {
			all.at(i)[0]->getPhysicsBody()->setVelocity(Vec2(v.x, 0));
			all.at(i)[1]->getPhysicsBody()->setVelocity(Vec2(v.x, 0));
			break;
		}
	}
}
