from django.db import models

# Create your models here.
class Color(models.Model):
    color_id = models.AutoField(primary_key=True)
    name = models.CharField(max_length = 50)

class State(models.Model):
    state_id = models.AutoField(primary_key=True)
    name = models.CharField(max_length = 50)

class Team(models.Model):
    team_id = models.AutoField(primary_key=True)
    name = models.CharField(max_length = 50)
    state_id = models.ForeignKey(State,on_delete=models.CASCADE)
    color_id = models.ForeignKey(Color,on_delete=models.CASCADE)
    wins = models.IntegerField()
    losses = models.IntegerField()

class Player(models.Model):
    player_id = models.AutoField(primary_key=True)
    team_id = models.ForeignKey(Team,on_delete=models.CASCADE)
    uniform_num = models.IntegerField()
    first_name = models.CharField(max_length = 50)
    last_name =  models.CharField(max_length = 50)
    mpg = models.IntegerField()
    ppg = models.IntegerField()
    rpg = models.IntegerField()
    apg = models.IntegerField()
    spg = models.FloatField()
    bpg = models.FloatField()

