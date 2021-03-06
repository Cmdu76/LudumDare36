#include "GameState.hpp"

GameState::GameState()
	: mWorld(ke::World::createInstance())
{
	createData();
	createActor();
	createGui();
}

GameState::~GameState()
{
	ke::World::destroyInstance();
}

bool GameState::handleEvent(sf::Event const& event)
{
	mWorld.handleEvent(event);
	mGui.handleEvent(event);

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && event.key.control)
	{
		mMoney1++;
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A && event.key.control)
	{
		end(1);
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::B && event.key.control)
	{
		end(2);
	}

	return false;
}

bool GameState::update(sf::Time dt)
{
	mWorld.update(dt);
	mMoneyText.setString("Money : " + std::to_string(mMoney1));

	getApplication().setDebugInfo("Money2", std::to_string(mMoney2));
	getApplication().setDebugInfo("GameTime", std::to_string(mGameTime.asSeconds()));
	getApplication().setDebugInfo("Score", std::to_string(computeScore()));

	mMoneyGranter += dt;
	if (mMoneyGranter > sf::seconds(0.5f))
	{
		grantMoney(1, ke::random(1, 3));
		grantMoney(2, ke::random(1, 3));
		mMoneyGranter = sf::Time::Zero;
	}

	updateAI(dt);

	mGameTime += dt;

	mScore.setString("Score : " + std::to_string(computeScore()));

	return false;
}

void GameState::render(sf::RenderTarget & target, sf::RenderStates states)
{
	mWorld.render(target);

	mGui.draw();

	if (mHead != 0)
	{
		target.draw(mHeadSprite);
	}

	if (mBody != 0)
	{
		target.draw(mBodySprite);
	}

	if (mLegs != 0)
	{
		target.draw(mLegsSprite);
	}
	target.draw(mMoneyText);
	target.draw(mScore);
}

void GameState::onActivate()
{
}

void GameState::onDeactivate()
{
}

void GameState::createData()
{
	mData[11] = GameData(11, 25, 50);
	mData[12] = GameData(12, 50, 80);
	mData[13] = GameData(13, 90, 125);
	mData[21] = GameData(21, 25, 25);
	mData[22] = GameData(22, 50, 37);
	mData[23] = GameData(23, 100, 60);
	mData[31] = GameData(31, 25, 100);
	mData[32] = GameData(32, 50, 150);
	mData[33] = GameData(33, 90, 210);

	mMoney1 = 150;
	mMoney2 = 150;
	mEnemyCost = 110;

	mHead = 0;
	mBody = 0;
	mLegs = 0;
}

void GameState::createActor()
{
	mWorld.createActor<MyMap>();

	mBase1 = mWorld.createActor<Base>(1)->getId();
	mBase2 = mWorld.createActor<Base>(2)->getId();
}

void GameState::createGui()
{
	tgui::ToolTip::setTimeToDisplay(sf::Time::Zero);

	ke::Texture& texture = mWorld.getResource<ke::Texture>("part");
	mHeadSprite.setTexture(texture);
	mHeadSprite.setTextureRect(sf::IntRect(0, 0, 74, 74));
	mHeadSprite.setPosition(299, 45 + 450);
	mBodySprite.setTexture(texture);
	mBodySprite.setTextureRect(sf::IntRect(0, 0, 74, 74));
	mBodySprite.setPosition(299, 171 + 450);
	mLegsSprite.setTexture(texture);
	mLegsSprite.setTextureRect(sf::IntRect(0, 0, 74, 74));
	mLegsSprite.setPosition(299, 297 + 450);

	ke::Font& font = mWorld.getResource<ke::Font>("futura");
	mMoneyText.setFont(font);
	mMoneyText.setFillColor(sf::Color::White);
	mMoneyText.setOutlineColor(sf::Color::Black);
	mMoneyText.setOutlineThickness(2.f);
	mMoneyText.setCharacterSize(30);
	mMoneyText.setString("Money : " + std::to_string(mMoney1));
	mMoneyText.setPosition(20, 390 + 450);

	mScore.setFont(font);
	mScore.setOutlineThickness(2.f);
	mScore.setOutlineColor(sf::Color::Black);
	mScore.setFillColor(sf::Color::White);
	mScore.setString("Score : " + std::to_string(computeScore()));
	mScore.setCharacterSize(40);
	mScore.setOrigin(mScore.getGlobalBounds().width * 0.5f, mScore.getGlobalBounds().height * 0.5f);
	mScore.setPosition(800, 20);

	ke::Theme& theme = mWorld.getResource<ke::Theme>("css");

	tgui::Picture::Ptr background = std::make_shared<tgui::Picture>("gui.png");
	background->setPosition(0, 450);
	background->setSize(1600, 450);
	mGui.add(background);

	tgui::Button::Ptr create = theme.create("Button");
	create->setText("Create");
	create->setPosition(40, 156 + 450);
	create->setSize(183, 99);
	create->connect("pressed", [&]()
	{
		if (mHead != 0 && mBody != 0 && mLegs != 0)
		{
			mWorld.createActor<Robot>(mHead, mBody, mLegs, mData[mHead].value, mData[mBody].value, mData[mLegs].value)->setPosition({ 50.f, 365.f });
			mHead = 0;
			mBody = 0;
			mLegs = 0;
			mWorld.getApplication().playSound("sspawnr");
		}
	});
	mGui.add(create);

	tgui::VerticalLayout::Ptr vlayout1 = std::make_shared<tgui::VerticalLayout>();
	vlayout1->setPosition(422 + 30, 450 + 40);
	vlayout1->setSize(280, 380);
	mGui.add(vlayout1);
	tgui::HorizontalLayout::Ptr h11 = std::make_shared<tgui::HorizontalLayout>();
	tgui::Button::Ptr i11 = theme.create("i11");
	tgui::Button::Ptr l11 = theme.create("sign");
	tgui::Button::Ptr b11 = theme.create("Button");
	tgui::Label::Ptr t11 = theme.create("ToolTip");
	vlayout1->add(h11);
	h11->add(i11);
	h11->add(l11);
	h11->add(b11);
	tgui::HorizontalLayout::Ptr h12 = std::make_shared<tgui::HorizontalLayout>();
	tgui::Button::Ptr i12 = theme.create("i12");
	tgui::Button::Ptr l12 = theme.create("sign");
	tgui::Button::Ptr b12 = theme.create("Button");
	tgui::Label::Ptr t12 = theme.create("ToolTip");
	vlayout1->add(h12);
	h12->add(i12);
	h12->add(l12);
	h12->add(b12);
	tgui::HorizontalLayout::Ptr h13 = std::make_shared<tgui::HorizontalLayout>();
	tgui::Button::Ptr i13 = theme.create("i13");
	tgui::Button::Ptr l13 = theme.create("sign");
	tgui::Button::Ptr b13 = theme.create("Button");
	tgui::Label::Ptr t13 = theme.create("ToolTip");
	vlayout1->add(h13);
	h13->add(i13);
	h13->add(l13);
	h13->add(b13);
	vlayout1->insertSpace(1, 0.1f);
	vlayout1->insertSpace(3, 0.1f);
	vlayout1->insertSpace(5, 0.1f);
	h11->insertSpace(1, 0.2f);
	h11->insertSpace(3, 0.2f);
	h11->insertSpace(5, 0.2f);
	h12->insertSpace(1, 0.2f);
	h12->insertSpace(3, 0.2f);
	h12->insertSpace(5, 0.2f);
	h13->insertSpace(1, 0.2f);
	h13->insertSpace(3, 0.2f);
	h13->insertSpace(5, 0.2f);
	t11->setText("Life : " + std::to_string(mData[11].value));
	i11->setToolTip(t11);
	i11->setSize(74, 74);
	l11->setSize(80, 80);
	l11->setText("$" + std::to_string(mData[11].price));
	l11->setTextSize(20);
	b11->setSize(80, 80);
	b11->setText("Buy");
	b11->setTextSize(20);
	t12->setText("Life : " + std::to_string(mData[12].value));
	i12->setToolTip(t12);
	i12->setSize(74, 74);
	l12->setSize(80, 80);
	l12->setText("$" + std::to_string(mData[12].price));
	l12->setTextSize(20);
	b12->setSize(80, 80);
	b12->setText("Buy");
	b12->setTextSize(20);
	t13->setText("Life : " + std::to_string(mData[13].value));
	i13->setToolTip(t13);
	i13->setSize(74, 74);
	l13->setSize(80, 80);
	l13->setText("$" + std::to_string(mData[13].price));
	l13->setTextSize(20);
	b13->setSize(80, 80);
	b13->setText("Buy");
	b13->setTextSize(20);

	tgui::VerticalLayout::Ptr vlayout2 = std::make_shared<tgui::VerticalLayout>();
	vlayout2->setPosition(767 + 30, 450 + 40);
	vlayout2->setSize(280, 380);
	mGui.add(vlayout2);
	tgui::HorizontalLayout::Ptr h21 = std::make_shared<tgui::HorizontalLayout>();
	tgui::Button::Ptr i21 = theme.create("i21");
	tgui::Button::Ptr l21 = theme.create("sign");
	tgui::Button::Ptr b21 = theme.create("Button");
	tgui::Label::Ptr t21 = theme.create("ToolTip");
	vlayout2->add(h21);
	h21->add(i21);
	h21->add(l21);
	h21->add(b21);
	tgui::HorizontalLayout::Ptr h22 = std::make_shared<tgui::HorizontalLayout>();
	tgui::Button::Ptr i22 = theme.create("i22");
	tgui::Button::Ptr l22 = theme.create("sign");
	tgui::Button::Ptr b22 = theme.create("Button");
	tgui::Label::Ptr t22 = theme.create("ToolTip");
	vlayout2->add(h22);
	h22->add(i22);
	h22->add(l22);
	h22->add(b22);
	tgui::HorizontalLayout::Ptr h23 = std::make_shared<tgui::HorizontalLayout>();
	tgui::Button::Ptr i23 = theme.create("i23");
	tgui::Button::Ptr l23 = theme.create("sign");
	tgui::Button::Ptr b23 = theme.create("Button");
	tgui::Label::Ptr t23 = theme.create("ToolTip");
	vlayout2->add(h23);
	h23->add(i23);
	h23->add(l23);
	h23->add(b23);
	vlayout2->insertSpace(1, 0.1f);
	vlayout2->insertSpace(3, 0.1f);
	vlayout2->insertSpace(5, 0.1f);
	h21->insertSpace(1, 0.2f);
	h21->insertSpace(3, 0.2f);
	h21->insertSpace(5, 0.2f);
	h22->insertSpace(1, 0.2f);
	h22->insertSpace(3, 0.2f);
	h22->insertSpace(5, 0.2f);
	h23->insertSpace(1, 0.2f);
	h23->insertSpace(3, 0.2f);
	h23->insertSpace(5, 0.2f);
	t21->setText("Attack : " + std::to_string(mData[21].value));
	i21->setToolTip(t21);
	i21->setSize(74, 74);
	l21->setSize(80, 80);
	l21->setText("$" + std::to_string(mData[21].price));
	l21->setTextSize(20);
	b21->setSize(80, 80);
	b21->setText("Buy");
	b21->setTextSize(20);
	t22->setText("Attack : " + std::to_string(mData[22].value));
	i22->setToolTip(t22);
	i22->setSize(74, 74);
	l22->setSize(80, 80);
	l22->setText("$" + std::to_string(mData[22].price));
	l22->setTextSize(20);
	b22->setSize(80, 80);
	b22->setText("Buy");
	b22->setTextSize(20);
	t23->setText("Attack : " + std::to_string(mData[23].value));
	i23->setToolTip(t23);
	i23->setSize(74, 74);
	l23->setSize(80, 80);
	l23->setText("$" + std::to_string(mData[23].price));
	l23->setTextSize(20);
	b23->setSize(80, 80);
	b23->setText("Buy");
	b23->setTextSize(20);

	tgui::VerticalLayout::Ptr vlayout3 = std::make_shared<tgui::VerticalLayout>();
	vlayout3->setPosition(1112 + 30, 450 + 40);
	vlayout3->setSize(280, 380);
	mGui.add(vlayout3);
	tgui::HorizontalLayout::Ptr h31 = std::make_shared<tgui::HorizontalLayout>();
	tgui::Button::Ptr i31 = theme.create("i31");
	tgui::Button::Ptr l31 = theme.create("sign");
	tgui::Button::Ptr b31 = theme.create("Button");
	tgui::Label::Ptr t31 = theme.create("ToolTip");
	vlayout3->add(h31);
	h31->add(i31);
	h31->add(l31);
	h31->add(b31);
	tgui::HorizontalLayout::Ptr h32 = std::make_shared<tgui::HorizontalLayout>();
	tgui::Button::Ptr i32 = theme.create("i32");
	tgui::Button::Ptr l32 = theme.create("sign");
	tgui::Button::Ptr b32 = theme.create("Button");
	tgui::Label::Ptr t32 = theme.create("ToolTip");
	vlayout3->add(h32);
	h32->add(i32);
	h32->add(l32);
	h32->add(b32);
	tgui::HorizontalLayout::Ptr h33 = std::make_shared<tgui::HorizontalLayout>();
	tgui::Button::Ptr i33 = theme.create("i33");
	tgui::Button::Ptr l33 = theme.create("sign");
	tgui::Button::Ptr b33 = theme.create("Button");
	tgui::Label::Ptr t33 = theme.create("ToolTip");
	vlayout3->add(h33);
	h33->add(i33);
	h33->add(l33);
	h33->add(b33);
	vlayout3->insertSpace(1, 0.1f);
	vlayout3->insertSpace(3, 0.1f);
	vlayout3->insertSpace(5, 0.1f);
	h31->insertSpace(1, 0.2f);
	h31->insertSpace(3, 0.2f);
	h31->insertSpace(5, 0.2f);
	h32->insertSpace(1, 0.2f);
	h32->insertSpace(3, 0.2f);
	h32->insertSpace(5, 0.2f);
	h33->insertSpace(1, 0.2f);
	h33->insertSpace(3, 0.2f);
	h33->insertSpace(5, 0.2f);
	t31->setText("Speed : " + std::to_string(mData[31].value));
	i31->setToolTip(t31);
	i31->setSize(74, 74);
	l31->setSize(80, 80);
	l31->setText("$" + std::to_string(mData[31].price));
	l31->setTextSize(20);
	b31->setSize(80, 80);
	b31->setText("Buy");
	b31->setTextSize(20);
	t32->setText("Speed : " + std::to_string(mData[32].value));
	i32->setToolTip(t32);
	i32->setSize(74, 74);
	l32->setSize(80, 80);
	l32->setText("$" + std::to_string(mData[32].price));
	l32->setTextSize(20);
	b32->setSize(80, 80);
	b32->setText("Buy");
	b32->setTextSize(20);
	t33->setText("Speed : " + std::to_string(mData[33].value));
	i33->setToolTip(t33);
	i33->setSize(74, 74);
	l33->setSize(80, 80);
	l33->setText("$" + std::to_string(mData[33].price));
	l33->setTextSize(20);
	b33->setSize(80, 80);
	b33->setText("Buy");
	b33->setTextSize(20);

	b11->connect("pressed", [&]()
	{
		if (mHead == 0 && mMoney1 >= mData[11].price)
		{
			mHead = 11;
			mMoney1 -= mData[11].price;
			mMoneySpend += mData[11].price;
			mHeadSprite.setTextureRect({ 0,0,74,74 });
			mWorld.getApplication().playSound("sgood");
		}
		else
		{
			mWorld.getApplication().playSound("sbad");
		}
	});
	b12->connect("pressed", [&]()
	{
		if (mHead == 0 && mMoney1 >= mData[12].price)
		{
			mHead = 12;
			mMoney1 -= mData[12].price;
			mMoneySpend += mData[12].price;
			mHeadSprite.setTextureRect({ 74,0,74,74 });
			mWorld.getApplication().playSound("sgood");
		}
		else
		{
			mWorld.getApplication().playSound("sbad");
		}
	});
	b13->connect("pressed", [&]()
	{
		if (mHead == 0 && mMoney1 >= mData[13].price)
		{
			mHead = 13;
			mMoney1 -= mData[13].price;
			mMoneySpend += mData[13].price;
			mHeadSprite.setTextureRect({ 148,0,74,74 });
			mWorld.getApplication().playSound("sgood");
		}
		else
		{
			mWorld.getApplication().playSound("sbad");
		}
	});
	b21->connect("pressed", [&]()
	{
		if (mBody == 0 && mMoney1 >= mData[21].price)
		{
			mBody = 21;
			mMoney1 -= mData[21].price;
			mMoneySpend += mData[21].price;
			mBodySprite.setTextureRect({ 0,74,74,74 });
			mWorld.getApplication().playSound("sgood");
		}
		else
		{
			mWorld.getApplication().playSound("sbad");
		}
	});
	b22->connect("pressed", [&]()
	{
		if (mBody == 0 && mMoney1 >= mData[22].price)
		{
			mBody = 22;
			mMoney1 -= mData[22].price;
			mMoneySpend += mData[22].price;
			mBodySprite.setTextureRect({ 74,74,74,74 });
			mWorld.getApplication().playSound("sgood");
		}
		else
		{
			mWorld.getApplication().playSound("sbad");
		}
	});
	b23->connect("pressed", [&]()
	{
		if (mBody == 0 && mMoney1 >= mData[23].price)
		{
			mBody = 23;
			mMoney1 -= mData[23].price;
			mMoneySpend += mData[23].price;
			mBodySprite.setTextureRect({ 148,74,74,74 });
			mWorld.getApplication().playSound("sgood");
		}
		else
		{
			mWorld.getApplication().playSound("sbad");
		}
	});
	b31->connect("pressed", [&]()
	{
		if (mLegs == 0 && mMoney1 >= mData[31].price)
		{
			mLegs = 31;
			mMoney1 -= mData[31].price;
			mMoneySpend += mData[31].price;
			mLegsSprite.setTextureRect({ 0,148,74,74 });
			mWorld.getApplication().playSound("sgood");
		}
		else
		{
			mWorld.getApplication().playSound("sbad");
		}
	});
	b32->connect("pressed", [&]()
	{
		if (mLegs == 0 && mMoney1 >= mData[32].price)
		{
			mLegs = 32;
			mMoney1 -= mData[32].price;
			mMoneySpend += mData[32].price;
			mLegsSprite.setTextureRect({ 74,148,74,74 });
			mWorld.getApplication().playSound("sgood");
		}
		else
		{
			mWorld.getApplication().playSound("sbad");
		}
	});
	b33->connect("pressed", [&]()
	{
		if (mLegs == 0 && mMoney1 >= mData[33].price)
		{
			mLegs = 33;
			mMoney1 -= mData[33].price;
			mMoneySpend += mData[33].price;
			mLegsSprite.setTextureRect({ 148,148,74,74 });
			mWorld.getApplication().playSound("sgood");
		}
		else
		{
			mWorld.getApplication().playSound("sbad");
		}
	});
}

void GameState::end(int team)
{
	getApplication().getValues().setProperty("winner", std::to_string(team));
	getApplication().getValues().setProperty("score", std::to_string(computeScore()));
	clearStates();
	pushState("EndState");
}

void GameState::grantMoney(int team, int amount)
{
	if (team == 1)
	{
		mMoney1 += amount;
	}
	if (team == 2)
	{
		mMoney2 += amount;
	}
}

void GameState::updateAI(sf::Time dt)
{
	if (mMoney2 >= mEnemyCost)
	{
		mMoney2 -= mEnemyCost;
		mWorld.createActor<Soldier>(mGameTime)->setPosition({ 1550.f, 365.f });
		mWorld.getApplication().playSound("sspawns");
	}
}

void GameState::killEnemy()
{
	mEnenyKilled++;
}

int GameState::computeScore()
{
	return (int)(10000 - mMoneySpend - mGameTime.asSeconds() + 25.f * mEnenyKilled);
}

GameState::GameData::GameData()
{
	id = 0;
	price = 0;
	value = 0;
}

GameState::GameData::GameData(int id, int price, int value)
{
	this->id = id;
	this->price = price;
	this->value = value;
}
