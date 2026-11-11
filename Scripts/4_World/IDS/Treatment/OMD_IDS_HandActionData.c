// Extra int on the action channel vanilla already uses for a craft recipe.
// Not a new RPC id. See ActionWorldCraft.WriteToContext.
// P:\scripts\4_world\classes\useractionscomponent\actions\continuous\actionworldcraft.c:196
class OMD_IDS_HandActionReciveData : ActionReciveData
{
	int m_HandVariant;
};

class OMD_IDS_HandActionData : ActionData
{
	int m_HandVariant;
	// One bandage action spends one portion, then the repeat stops.
	int m_Spent;
	// Suture shock clock. Server action data only. Not written to the action channel and not saved.
	int m_SutureOn;
	int m_SutureZone;
	int m_SutureStartMs;
	int m_SutureDue;
	float m_SutureDuration;
	// Extract shock clock. Server action data only. Not written to the action channel and not saved.
	int m_ExtractOn;
	int m_ExtractZone;
	int m_ExtractStartMs;
	int m_ExtractDue;
	float m_ExtractDuration;
};
