#include "g_local.h"

void ChasecamTrack(edict_t *ent);

/* The ent os the owner of the chasecam */

void ChasecamStart(edict_t *ent)
{
	edict_t *chasecam;
	int offset[] = { 0, 0, 40 };
	vec3_t forward, right, up;
	vec3_t cf, cr, cu;
	ent->client->chasetoggle = 1;

	ent->client->ps.gunindex = 0;

	chasecam = G_Spawn();
	chasecam->owner = ent;
	chasecam->solid = SOLID_NOT;
	chasecam->movetype = MOVETYPE_FLYMISSILE;
	
	ent->client->ps.pmove.pm_flags |= PMF_NO_PREDICTION;

	ent->svflags |= SVF_NOCLIENT;
	
	//VectorCopy(ent->s.angles, chasecam->s.angles);

	VectorClear(chasecam->mins);
	VectorClear(chasecam->maxs);

	//VectorCopy(ent->s.origin, chasecam->s.origin);
	//VectorAdd(chasecam->s.origin, offset, chasecam->s.origin);
	chasecam->classname = "chasecam";
	AngleVectors(ent->client->v_angle, forward, right, up);
	VectorMA(ent->s.origin, -60, forward, chasecam->s.origin);
	VectorAdd(chasecam->s.origin, offset, chasecam->s.origin);
	chasecam->prethink = ChasecamTrack;
	//chasecam->nextthink = level.time + .1;
	ent->client->chasecam = chasecam;
	ent->client->oldplayer = G_Spawn();

}

void ChasecamRestart(edict_t *ent)
{
	if (ent->owner->health <= 0)
	{
		G_FreeEdict(ent);
		return;
	}
	if (ent->owner->waterlevel)
		return;
	ChasecamStart(ent->owner);
	G_FreeEdict(ent);
}

void ChasecamRemove(edict_t *ent, char *opt)
{
	VectorClear(ent->client->chasecam->velocity);
	ent->client->ps.gunindex = gi.modelindex(ent->client->pers.weapon->view_model);
	ent->s.modelindex = ent->client->oldplayer->s.modelindex;

	ent->svflags &= ~SVF_NOCLIENT;

	if (!strcmp(opt, "background"))
	{
		ent->client->chasetoggle = 0;
		G_FreeEdict(ent->client->chasecam);
		G_FreeEdict(ent->client->oldplayer);
		ent->client->chasecam = G_Spawn();
		ent->client->chasecam->owner = ent;
		ent->client->chasecam->solid = SOLID_NOT;
		ent->client->chasecam->movetype = MOVETYPE_FLYMISSILE;
		VectorClear(ent->client->chasecam->mins);
		VectorClear(ent->client->chasecam->maxs);
		ent->client->chasecam->classname = "chasecam";
		ent->client->chasecam->prethink = ChasecamRestart;
	}
	else if (!strcmp(opt, "off"))

	{

		// Added by WarZone - Begin
		if (ent->client->chasetoggle)
		{
			free(ent->client->oldplayer->client); // needed??
			G_FreeEdict(ent->client->oldplayer);
		}
		ent->client->chasetoggle = 0;
		G_FreeEdict(ent->client->chasecam);
		// Added by WarZone - End

	}

}

void ChasecamTrack(edict_t *ent)
{
	trace_t tr;
	vec3_t spot1, spot2, dir;
	vec3_t forward, right, up;
	int distance;
	int tot;
	int offset[] = { 0, 0, 40 };
	//if(ent->owner->velocity[0] == 0 && ent->velocity[1] == 0 && ent->owner->velocity[2] == 0)
	//VectorCopy(ent->owner->s.origin, ent->s.origin);
	//VectorAdd(ent->s.origin, offset, ent->s.origin);
	
	//ent->velocity[0] = ent->owner->velocity[0];
	//ent->velocity[1] = ent->owner->velocity[1];
	//ent->velocity[2] = 0;
	ent->nextthink = level.time + .1;
	AngleVectors(ent->owner->client->v_angle, forward, right, up);
	//AngleVectors(ent->owner->client->v_angle, forward, right, up);
	VectorMA(ent->owner->s.origin, -60, forward, ent->s.origin);
	VectorAdd(ent->s.origin, offset, ent->s.origin);
	
}
void Cmd_Chasecam_Toggle(edict_t *ent)
{
	// Added by WarZone - Begin
	if (!ent->waterlevel && !ent->deadflag)
	{
		/*if (ent->client->chasetoggle)
			ChasecamRemove(ent, "off");
		else*/
			ChasecamStart(ent);
	}
	else if (ent->waterlevel && !ent->deadflag)
		gi.cprintf(ent, PRINT_HIGH, "Camera cannot be modified while in water\n");
	// Added by WarZone - End
}

void CheckChasecam_Viewent(edict_t *ent)

{
	// Added by WarZone - Begin
	gclient_t       *cl;

	if (!ent->client->oldplayer->client)
	{
		cl = (gclient_t *)malloc(sizeof(gclient_t));
		ent->client->oldplayer->client = cl;
	}
	// Added by WarZone - End

	if ((ent->client->chasetoggle == 1) && (ent->client->oldplayer))

	{

		ent->client->oldplayer->s.frame = ent->s.frame;
		VectorCopy(ent->s.origin, ent->client->oldplayer->s.origin);

		VectorCopy(ent->velocity, ent->client->oldplayer->velocity);

		VectorCopy(ent->s.angles, ent->client->oldplayer->s.angles);
		// Added by WarZone - Begin
		ent->client->oldplayer->s = ent->s; // copies over all of the important player related information
		// Added by WarZone - End

		gi.linkentity(ent->client->oldplayer);

	}



}