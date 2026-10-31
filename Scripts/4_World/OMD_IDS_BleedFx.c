// One bleeding particle per vanilla bone on this player.
// Heavy replaces light. A cleared zone removes the particle when the bone is free.
class OMD_IDS_BleedFx
{
	static const int KIND_LIGHT = 1;
	static const int KIND_HEAVY = 2;

	protected ref array<string> m_Bones;
	protected ref array<int> m_Kinds;
	protected ref array<ref EffectParticle> m_Effects;
	protected ref array<string> m_WantBones;
	protected ref array<int> m_WantKinds;
	protected ref array<vector> m_WantOri;
	protected ref array<vector> m_WantPos;

	void OMD_IDS_BleedFx()
	{
		m_Bones = new array<string>();
		m_Kinds = new array<int>();
		m_Effects = new array<ref EffectParticle>();
		m_WantBones = new array<string>();
		m_WantKinds = new array<int>();
		m_WantOri = new array<vector>();
		m_WantPos = new array<vector>();
	}

	void Apply(PlayerBase player, int lightMask, int heavyMask)
	{
		if (!player)
		{
			return;
		}

		if (!GetGame())
		{
			return;
		}

		if (GetGame().IsDedicatedServer())
		{
			return;
		}

		CollectWants(lightMask, heavyMask);
		Reconcile(player);
	}

	void Drop()
	{
		int index;
		EffectParticle effect;

		if (!m_Effects)
		{
			return;
		}

		for (index = m_Effects.Count() - 1; index >= 0; index = index - 1)
		{
			effect = m_Effects.Get(index);
			SEffectManager.DestroyEffect(effect);
		}

		m_Bones.Clear();
		m_Kinds.Clear();
		m_Effects.Clear();
	}

	protected void CollectWants(int lightMask, int heavyMask)
	{
		int zoneId;
		int bit;
		int kind;
		string bone;
		vector orientation;
		vector offset;

		m_WantBones.Clear();
		m_WantKinds.Clear();
		m_WantOri.Clear();
		m_WantPos.Clear();
		for (zoneId = OMD_IDS_ZoneId.HEAD; zoneId <= OMD_IDS_ZoneId.RIGHT_FOOT; zoneId = zoneId + 1)
		{
			bit = 1 << (zoneId - 1);
			kind = 0;
			if ((heavyMask & bit) != 0)
			{
				kind = KIND_HEAVY;
			}

			if (kind == 0)
			{
				if ((lightMask & bit) != 0)
				{
					kind = KIND_LIGHT;
				}
			}

			if (kind == 0)
			{
				continue;
			}

			if (!OMD_IDS_BleedPoint.Find(zoneId, bone, orientation, offset))
			{
				continue;
			}

			AddWant(bone, kind, orientation, offset);
		}
	}

	protected void AddWant(string bone, int kind, vector orientation, vector offset)
	{
		int index;

		index = FindWant(bone);
		if (index >= 0)
		{
			if (kind == KIND_HEAVY)
			{
				m_WantKinds.Set(index, KIND_HEAVY);
			}

			return;
		}

		m_WantBones.Insert(bone);
		m_WantKinds.Insert(kind);
		m_WantOri.Insert(orientation);
		m_WantPos.Insert(offset);
	}

	protected int FindWant(string bone)
	{
		int index;

		for (index = 0; index < m_WantBones.Count(); index = index + 1)
		{
			if (m_WantBones.Get(index) == bone)
			{
				return index;
			}
		}

		return -1;
	}

	protected void Reconcile(PlayerBase player)
	{
		int index;
		int wantIndex;
		string bone;

		for (index = m_Bones.Count() - 1; index >= 0; index = index - 1)
		{
			bone = m_Bones.Get(index);
			wantIndex = FindWant(bone);
			if (wantIndex < 0)
			{
				DestroyAt(index);
				continue;
			}

			if (m_Kinds.Get(index) != m_WantKinds.Get(wantIndex))
			{
				DestroyAt(index);
				continue;
			}

			m_WantKinds.Set(wantIndex, 0);
		}

		for (index = 0; index < m_WantBones.Count(); index = index + 1)
		{
			if (m_WantKinds.Get(index) == 0)
			{
				continue;
			}

			CreateOne(player, m_WantBones.Get(index), m_WantKinds.Get(index), m_WantOri.Get(index), m_WantPos.Get(index));
		}
	}

	protected void CreateOne(PlayerBase player, string bone, int kind, vector orientation, vector offset)
	{
		int particleId;
		int boneIdx;
		float time;
		vector pos;
		Particle particle;
		EffectParticle effect;

		if (!player)
		{
			return;
		}

		if (bone == "")
		{
			return;
		}

		boneIdx = player.GetBoneIndexByName(bone);
		if (boneIdx < 0)
		{
			return;
		}

		particleId = ParticleList.BLEEDING_SOURCE_LIGHT;
		if (kind == KIND_HEAVY)
		{
			particleId = ParticleList.BLEEDING_SOURCE;
		}

		effect = new EffectParticle();
		effect.SetParticleID(particleId);
		SEffectManager.PlayInWorld(effect, "0 0 0");
		particle = effect.GetParticle();
		if (!particle)
		{
			SEffectManager.DestroyEffect(effect);
			return;
		}

		particle.SetOrientation(orientation);
		pos = "0 0 0";
		pos += offset;
		particle.SetPosition(pos);
		time = Math.RandomFloat01() * 2;
		particle.SetParameter(-1, EmitorParam.CURRENT_TIME, time);
		player.AddChild(particle, boneIdx);
		m_Bones.Insert(bone);
		m_Kinds.Insert(kind);
		m_Effects.Insert(effect);
	}

	protected void DestroyAt(int index)
	{
		EffectParticle effect;

		effect = m_Effects.Get(index);
		SEffectManager.DestroyEffect(effect);
		m_Bones.RemoveOrdered(index);
		m_Kinds.RemoveOrdered(index);
		m_Effects.RemoveOrdered(index);
	}
};
