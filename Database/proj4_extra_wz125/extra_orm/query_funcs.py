import django
from orm.models import Player, State, Team, Color

def query1(use_mpg, min_mpg, max_mpg, use_ppg, min_ppg, max_ppg, use_rpg, min_rpg, max_rpg, use_apg, min_apg, max_apg, use_spg, min_spg, max_spg, use_bpg, min_bpg, max_bpg):
    result = Player.objects.all()
    if use_mpg:
        result = Player.objects.filter(mpg__lte = max_mpg, mpg__gte = min_mpg)
    if use_ppg:
        result = result.filter(ppg__lte = max_ppg, ppg__gte = min_ppg)
    if use_rpg:
        result = result.filter(rpg__lte = max_rpg, rpg__gte = min_rpg)
    if use_apg:
        result = result.filter(apg__lte = max_apg, apg__gte = min_apg)
    if use_spg:
        result = result.filter(spg__lte = max_spg, spg__gte = min_spg)
    if use_bpg:
        result = result.filter(bpg__lte = max_bpg, bpg__gte = min_bpg)
    print("PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG")
    for entries in result:
        print(entries.player_id, entries.team_id.team_id, entries.uniform_num, entries.first_name, entries.last_name, entries.mpg, entries.ppg, entries.rpg, entries.apg, entries.spg, entries.bpg)
         
def query2(team_color):
    result = Team.objects.filter(color_id__name = team_color)
    print("NAME")
    for entries in result:
        print(entries.name)
        
def query3(team_name):
    result = Player.objects.filter(team_id__name = team_name).order_by('-ppg')
    print("FIRST_NAME LAST_NAME")
    for entries in result:
        print(entries.first_name, entries.last_name)
        
def query4(team_state, team_color):
    result = Player.objects.filter(team_id__state_id__name = team_state).filter(team_id__color_id__name = team_color)
    print("FISRT_NAME LAST_NAME UNIFORM_NUM")
    for entries in result:
        print(entries.first_name, entries.last_name, entries.uniform_num)
        
def query5(num_wins):
    result = Player.objects.filter(team_id__wins__gt = num_wins)
    print("FISRT_NAME LAST_NAME NAME WINS")
    for entries in result:
        print(entries.first_name, entries.last_name, entries.team_id.name, entries.team_id.wins)
    
def add_player(team_id,jersey_num,first_name,last_name,mpg,ppg,rpg,apg,spg,bpg):
    Player.objects.get_or_create(team_id = Team.objects.get(team_id = team_id),uniform_num = jersey_num, first_name = first_name, last_name = last_name, mpg = mpg, ppg = ppg, rpg = rpg, apg = apg, spg = spg, bpg = bpg)

def add_team(name, state_id, color_id, wins, losses):
    Team.objects.get_or_create(name = name, state_id = State.objects.get(state_id = state_id), color_id = Color.objects.get(color_id = color_id), wins=wins, losses = losses)

def add_state(name):
    State.objects.get_or_create(name = name)

def add_color(name):
    Color.objects.get_or_create(name = name)
