# Generated by Django 3.0.2 on 2020-03-10 01:37

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('orm', '0002_player_state_team'),
    ]

    operations = [
        migrations.RenameField(
            model_name='player',
            old_name='wins',
            new_name='uniform_num',
        ),
    ]
