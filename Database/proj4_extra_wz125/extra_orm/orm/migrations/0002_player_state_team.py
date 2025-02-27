# Generated by Django 3.0.2 on 2020-03-10 01:33

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('orm', '0001_initial'),
    ]

    operations = [
        migrations.CreateModel(
            name='State',
            fields=[
                ('state_id', models.AutoField(primary_key=True, serialize=False)),
                ('name', models.CharField(max_length=50)),
            ],
        ),
        migrations.CreateModel(
            name='Team',
            fields=[
                ('team_id', models.AutoField(primary_key=True, serialize=False)),
                ('name', models.CharField(max_length=50)),
                ('wins', models.IntegerField()),
                ('losses', models.IntegerField()),
                ('color_id', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='orm.Color')),
                ('state_id', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='orm.State')),
            ],
        ),
        migrations.CreateModel(
            name='Player',
            fields=[
                ('player_id', models.AutoField(primary_key=True, serialize=False)),
                ('wins', models.IntegerField()),
                ('first_name', models.CharField(max_length=50)),
                ('last_name', models.CharField(max_length=50)),
                ('mpg', models.IntegerField()),
                ('ppg', models.IntegerField()),
                ('rpg', models.IntegerField()),
                ('apg', models.IntegerField()),
                ('spg', models.FloatField()),
                ('bpg', models.FloatField()),
                ('team_id', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='orm.Team')),
            ],
        ),
    ]
