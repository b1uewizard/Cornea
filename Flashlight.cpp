//================================================//

#include "Flashlight.hpp"

//================================================//

Flashlight::Flashlight(Ogre::SceneManager* mgr, Ogre::SceneNode* parent)
{
	m_pSceneMgr = mgr;

	m_attenuationRange = 1000.0;
	m_outerAngle = Ogre::Degree(20);

	// Flashlight settings
	m_pLight = m_pSceneMgr->createLight("Flashlight");
	m_pLight->setType(Ogre::Light::LT_SPOTLIGHT);
	m_pLight->setDiffuseColour(1.0, 1.0, 1.0);
	m_pLight->setSpecularColour(0.8, 0.8, 0.8);
	m_pLight->setCastShadows(Settings::getSingletonPtr()->graphics.shadows.enabled);
	m_pLight->setSpotlightInnerAngle(Ogre::Degree(5));
	m_pLight->setSpotlightOuterAngle(Ogre::Degree(21));
	m_pLight->setSpotlightFalloff(5.0);
	m_pLight->setAttenuation(0.0, 0.5, 0, 0);
	m_pLight->setDirection(Ogre::Vector3(0.0, 0.0, -1.0));

	// Set up scene node
	parent->createChildSceneNode("Flashlight")->attachObject(m_pLight);
	m_pSceneMgr->getSceneNode("Flashlight")->translate(5.0, -2.5, 0.0, Ogre::Node::TS_LOCAL);

	// Set default setting to off
	m_pLight->setVisible(false);
	m_state = STATE_IDLE;
}

//================================================//

Flashlight::~Flashlight(void)
{

}

//================================================//

void Flashlight::switchState(void)
{
	if(m_pLight->getVisible()){
		m_state = STATE_TURNING_OFF;
	}
	else{
		m_state = STATE_TURNING_ON;
		m_pLight->setAttenuation(0.0, 0.5, 0.0, 0.0);
		m_pLight->setVisible(true);
	}
}

//================================================//

void Flashlight::update(double timeSinceLastFrame)
{
	// Fade light in and out
	if(m_state == STATE_TURNING_ON){
		Ogre::Real range = m_pLight->getAttenuationRange();
		if(range < m_attenuationRange){
			range += (timeSinceLastFrame * 2.5);
		}
		else{
			range = m_attenuationRange;
			m_state = STATE_IDLE;
		}

		m_pLight->setAttenuation(range, 0.5, 0.0, 0.0);
	}
	else if(m_state == STATE_TURNING_OFF){
		Ogre::Real range = m_pLight->getAttenuationRange();
		if(range > 0.0){
			range -= (timeSinceLastFrame * 4.0);
			m_pLight->setAttenuation(range, 0.5, 0.0, 0.0);
		}
		else{
			m_pLight->setVisible(false);
			m_state = STATE_IDLE;
		}
	}
}

//================================================//