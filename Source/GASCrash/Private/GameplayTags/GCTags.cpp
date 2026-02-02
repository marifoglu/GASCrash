#include "GameplayTags/GCTags.h"

namespace GCTags
{
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
		
		namespace Enemy
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(HitReact, "GCTags.Events.Enemy.HitReact", "Tag for HitReact Event for Enemy")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(EndAttack, "GCTags.Events.Enemy.EndAttack", "Tag for EndAttack Event for Enemy")

		}
	}
}
