import os
os.environ.setdefault("DJANGO_SETTINGS_MODULE", "extra_orm.settings")

import django
django.setup()

import django
if django.VERSION >= (1, 7):
    django.setup()

import manage
import query_funcs
from orm.models import Color,State,Team,Player



def main():
    buildcolor()
    buildstate()
    buildteam()
    buildplayer()
    test()
    
def test():
    print("TEST QUERY1:\n\n")
    query_funcs.query1(1,10,20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
    print("\n")
    query_funcs.query1(1, 15, 20, 0, 0, 0, 1, 5, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0)
    print("TEST QUERY2:\n\n")
    query_funcs.query2("LightBlue")
    print("\n")
    print("[TEST] Non-exist")
    query_funcs.query2("Pink")
    print("TEST QUERY3:\n\n")
    query_funcs.query3("Duke")
    print("\n")
    query_funcs.query3("VirginiaTech")
    print("\n")
    print("[TEST] Non-exist")
    query_funcs.query3("Chinese")
    print("TEST QUERY4:\n\n")
    query_funcs.query4("NC", "Red")
    print("\n")
    query_funcs.query4("NC", "DarkBlue")
    print("\n")
    print("[TEST] Non-exist")
    query_funcs.query4("BB", "Orange")
    print("TEST QUERY5:\n\n")
    print("[TEST] Non-exist")
    query_funcs.query5(100)
    print("\n")
    query_funcs.query5(11)

    query_funcs.add_player(1, 1, "Jerome", "Robinson", 34, 19, 4, 3, 1.7, 0.4)
    query_funcs.add_player(2, 1, "Jer", "J'Robinson", 10, 20, 4, 3, 1.7, 0.4)
    query_funcs.add_team("A'AA", 3, 2, 3, 5)
    query_funcs.add_state("S'tateAAA")
    query_funcs.add_color("C'olor")

    
def buildcolor():
    f = open("color.txt")
    for line in f:
        color_id, name = line.split(' ')
        Color.objects.create(name=name[0:-1])
    f.close()

def buildstate():
    f = open("state.txt")
    for line in f:
        state_id, name = line.split(' ')
        State.objects.create(name=name[0:-1])
    f.close()
    
def buildteam():
    f = open("team.txt")
    for line in f:
        team_id, name,state_id,color_id,wins,losses = line.split(' ')
        Team.objects.create(name=name, state_id = State.objects.get(state_id = state_id), color_id=Color.objects.get(color_id = color_id), wins=wins, losses=losses)
    f.close()
    
def buildplayer():
    f = open("player.txt")
    for line in f:
        player_id, team_id, uniform_num,first_name,last_name, mpg,ppg, rpg, apg, spg, bpg = line.split(' ')
        Player.objects.create(team_id = Team.objects.get(team_id = team_id), uniform_num = uniform_num, first_name = first_name, last_name = last_name, mpg = mpg, ppg = ppg, rpg = rpg, apg = apg, spg = spg, bpg = bpg)
    f.close()
    
if __name__ == "__main__":
    main()
    print("Done!")
