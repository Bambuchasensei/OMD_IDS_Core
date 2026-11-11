#ifndef SERVER

modded class MissionGameplay
{
	private bool m_OMD_IDS_SnapshotRequested;
	private bool m_OMD_IDS_WasRaised;
	private bool m_OMD_IDS_PainTraceInit;
	private bool m_OMD_IDS_PainTraceWant;
	private bool m_OMD_IDS_PainTraceRunning;
	private bool m_OMD_IDS_PainTraceFaded;
	private float m_OMD_IDS_PainTraceChromMax;
	private float m_OMD_IDS_PainTraceGaussMax;

	override void OnInit()
	{
		super.OnInit();
		OMD_IDS_HudManager.Create();
	}

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
		OMD_IDS_UpdatePainVisual();
		OMD_IDS_UpdatePainJerk();
		OMD_IDS_WatchAim();
		OMD_IDS_HudManager manager = OMD_IDS_HudManager.Get();
		if (!manager)
		{
			return;
		}

		RequestSnapshotOnce();
		manager.UpdatePain(timeslice);
		OMD_IDS_InjuryMailbox mailbox = OMD_IDS_InjuryMailbox.Get();
		if (!mailbox)
		{
			return;
		}

		if (!mailbox.HasPending())
		{
			return;
		}

		if (!LocalPlayerReady())
		{
			return;
		}

		array<ref OMD_IDS_InjurySnapshotEntry> snapshot = mailbox.Take();
		OMD_IDS_LogSnapshot(snapshot);
		array<ref OMD_IDS_HudDamageEntry> entries = OMD_IDS_InjuryHudBridge.ToEntries(snapshot);
		manager.SetActiveDamages(entries);
	}

	// One line, only while hud.json debugEnabled is on. Lists the raw snapshot before the HUD hides rows.
	private void OMD_IDS_LogSnapshot(array<ref OMD_IDS_InjurySnapshotEntry> snapshot)
	{
		int index;
		int rows;
		string line;
		OMD_IDS_HudManager manager;
		OMD_IDS_InjurySnapshotEntry row;

		manager = OMD_IDS_HudManager.Get();
		if (!manager)
		{
			return;
		}

		if (!manager.IsDebugEnabled())
		{
			return;
		}

		rows = 0;
		if (snapshot)
		{
			rows = snapshot.Count();
		}

		line = "[OMD_IDS] client snapshot rows=";
		line = line + rows.ToString();
		if (!snapshot)
		{
			Print(line);
			return;
		}

		for (index = 0; index < snapshot.Count(); index = index + 1)
		{
			row = snapshot.Get(index);
			if (!row)
			{
				continue;
			}

			line = line + " typeId=";
			line = line + row.typeId.ToString();
			line = line + " groupId=";
			line = line + row.groupId.ToString();
			line = line + " count=";
			line = line + row.count.ToString();
			line = line + ";";
		}

		Print(line);
	}

	override void OnMissionFinish()
	{
		OMD_IDS_InjuryMailbox mailbox = OMD_IDS_InjuryMailbox.Get();
		OMD_IDS_PainStore painStore;
		if (mailbox)
		{
			mailbox.Clear();
		}

		painStore = OMD_IDS_PainStore.Get();
		if (painStore)
		{
			painStore.Replace(0, null, null);
		}

		OMD_IDS_PainJerk.Reset();
		OMD_IDS_ClearPainFx();
		OMD_IDS_StopPainVisual();

		m_OMD_IDS_SnapshotRequested = false;
		m_OMD_IDS_WasRaised = false;
		OMD_IDS_HudManager.Destroy();
		super.OnMissionFinish();
	}

	private void OMD_IDS_UpdatePainJerk()
	{
		PlayerBase player;

		if (!GetGame())
		{
			return;
		}

		if (!Class.CastTo(player, GetGame().GetPlayer()))
		{
			return;
		}

		OMD_IDS_PainJerk.TryHigh(player);
	}

	private void OMD_IDS_UpdatePainVisual()
	{
		PlayerBase player;
		OMD_IDS_PainEffectState fx;
		PPERequester_OMD_IDS_Pain requester;
		OMD_IDS_HudManager manager;
		bool want;
		bool started;
		bool stopped;
		bool debugOn;
		float chromMin;
		float chromMax;
		float gaussMin;
		float gaussMax;
		float waveStep;

		if (!GetGame())
		{
			return;
		}

		requester = PPERequester_OMD_IDS_Pain.Cast(PPERequesterBank.GetRequester(PPERequester_OMD_IDS_Pain));
		if (!requester)
		{
			return;
		}

		debugOn = false;
		manager = OMD_IDS_HudManager.Get();
		if (manager)
		{
			debugOn = manager.IsDebugEnabled();
		}

		requester.OMD_IDS_SetDebugEnabled(debugOn);
		want = false;
		started = false;
		stopped = false;
		chromMin = 0;
		chromMax = 0;
		gaussMin = 0;
		gaussMax = 0;
		waveStep = 0;
		if (Class.CastTo(player, GetGame().GetPlayer()))
		{
			fx = player.OMD_IDS_GetPainFx();
			if (fx)
			{
				chromMin = fx.GetChromMin();
				chromMax = fx.GetChromMax();
				gaussMin = fx.GetGaussMin();
				gaussMax = fx.GetGaussMax();
				waveStep = fx.GetWaveStep();
				if (chromMax > 0)
				{
					want = true;
				}

				if (gaussMax > 0)
				{
					want = true;
				}
			}
		}

		if (want)
		{
			if (!requester.IsRequesterRunning())
			{
				requester.Start();
				started = true;
			}

			requester.SetVisual(chromMin, chromMax, gaussMin, gaussMax, waveStep);
		}
		else
		{
			if (requester.IsRequesterRunning())
			{
				requester.SetVisual(0, 0, 0, 0, 0);
				if (requester.OMD_IDS_IsFaded())
				{
					requester.Stop();
					stopped = true;
				}
			}
		}

		OMD_IDS_TracePainVisual(want, requester.IsRequesterRunning(), requester.OMD_IDS_IsFaded(), started, stopped, chromMax, gaussMax);
	}

	private void OMD_IDS_TracePainVisual(bool want, bool running, bool faded, bool started, bool stopped, float chromMax, float gaussMax)
	{
		OMD_IDS_HudManager manager;
		bool enabled;
		bool changed;

		enabled = false;
		manager = OMD_IDS_HudManager.Get();
		if (manager)
		{
			enabled = manager.IsDebugEnabled();
		}

		if (!enabled)
		{
			m_OMD_IDS_PainTraceInit = false;
			return;
		}

		changed = !m_OMD_IDS_PainTraceInit;
		if (want != m_OMD_IDS_PainTraceWant)
		{
			changed = true;
		}

		if (running != m_OMD_IDS_PainTraceRunning)
		{
			changed = true;
		}

		if (faded != m_OMD_IDS_PainTraceFaded)
		{
			changed = true;
		}

		if (chromMax != m_OMD_IDS_PainTraceChromMax)
		{
			changed = true;
		}

		if (gaussMax != m_OMD_IDS_PainTraceGaussMax)
		{
			changed = true;
		}

		if (started)
		{
			changed = true;
		}

		if (stopped)
		{
			changed = true;
		}

		if (!changed)
		{
			return;
		}

		m_OMD_IDS_PainTraceInit = true;
		m_OMD_IDS_PainTraceWant = want;
		m_OMD_IDS_PainTraceRunning = running;
		m_OMD_IDS_PainTraceFaded = faded;
		m_OMD_IDS_PainTraceChromMax = chromMax;
		m_OMD_IDS_PainTraceGaussMax = gaussMax;
		Print(string.Format("[OMD_IDS][Pain][PPE] mission want=%1 running=%2 faded=%3 chromMax=%4 gaussMax=%5 start=%6 stop=%7", want, running, faded, chromMax, gaussMax, started, stopped));
	}

	private void OMD_IDS_StopPainVisual()
	{
		PPERequester_OMD_IDS_Pain requester;
		OMD_IDS_HudManager manager;

		requester = PPERequester_OMD_IDS_Pain.Cast(PPERequesterBank.GetRequester(PPERequester_OMD_IDS_Pain));
		if (!requester)
		{
			return;
		}

		if (!requester.IsRequesterRunning())
		{
			return;
		}

		manager = OMD_IDS_HudManager.Get();
		if (manager)
		{
			if (manager.IsDebugEnabled())
			{
				Print("[OMD_IDS][Pain][PPE] mission finish stop");
			}
		}

		requester.Stop();
	}

	private void OMD_IDS_WatchAim()
	{
		PlayerBase player;
		bool raised;

		if (!GetGame())
		{
			return;
		}

		if (!Class.CastTo(player, GetGame().GetPlayer()))
		{
			m_OMD_IDS_WasRaised = false;
			return;
		}

		raised = player.IsFireWeaponRaised();
		if (raised)
		{
			if (!m_OMD_IDS_WasRaised)
			{
				OMD_IDS_PainJerk.Try(player);
			}
		}

		m_OMD_IDS_WasRaised = raised;
	}

	private void OMD_IDS_ClearPainFx()
	{
		PlayerBase player;

		if (!GetGame())
		{
			return;
		}

		if (!Class.CastTo(player, GetGame().GetPlayer()))
		{
			return;
		}

		if (!player.OMD_IDS_GetPainFx())
		{
			return;
		}

		player.OMD_IDS_GetPainFx().Clear();
	}

	private void RequestSnapshotOnce()
	{
		PlayerBase player;
		ScriptRPC rpc;

		if (m_OMD_IDS_SnapshotRequested)
		{
			return;
		}

		if (!LocalPlayerReady())
		{
			return;
		}

		if (!Class.CastTo(player, GetGame().GetPlayer()))
		{
			return;
		}

		rpc = new ScriptRPC();
		rpc.Send(player, OMD_IDS_Rpc.SNAPSHOT_REQUEST, true, null);
		m_OMD_IDS_SnapshotRequested = true;
	}

	private bool LocalPlayerReady()
	{
		PlayerBase player;

		if (!GetGame())
		{
			return false;
		}

		if (!Class.CastTo(player, GetGame().GetPlayer()))
		{
			return false;
		}

		if (!player.IsControlledPlayer())
		{
			return false;
		}

		return true;
	}
};

#endif
