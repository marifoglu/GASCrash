#include "GameplayTags/GCTags.h"

namespace GCTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(None, "GCTags.None", "None")
	
	namespace SetByCallers
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Projectile, "GCTags.SetByCallers.Projectile", "Tag for the Set By Caller Magnitude for Projectile")

	}
	namespace GCAbilities
	{
		// Tag and value
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActivateOnGiven, "GCTags.Abilities.ActivateOnGiven", "Tag for the Ability that should be activated on given")
		
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Primary, "GCTags.Abilities.Primary", "Tag for the Primary Ability")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Secondary, "GCTags.Abilities.Secondary", "Tag for the Secondary Ability")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tertiary, "GCTags.Abilities.Tertiary", "Tag for the Tertiary Ability")

		namespace Enemy
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attack, "GCTags.Abilities.Enemy.Attack", "Enemy Attack Tag")
		}
		
	}
	namespace Events
	{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(KillScored, "GCTags.Events.KillScored", "Tag for KillScored Event")
		
		namespace Player
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(HitReact, "GCTags.Events.Player.HitReact", "Tag for the Player HitReact Event")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Death, "GCTags.Events.Player.Death", "Tag for the Player Death Event")
		}
		
		namespace Enemy
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(HitReact, "GCTags.Events.Enemy.HitReact", "Tag for the Enemy HitReact Event")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(EndAttack, "GCTags.Events.Enemy.EndAttack", "Tag for the Enemy EndAttack Event")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(MeleeTraceHit, "GCTags.Events.Enemy.MeleeTraceHit", "Tag for the Enemy Melee Trace Hit Tag")
		}
	}
}
