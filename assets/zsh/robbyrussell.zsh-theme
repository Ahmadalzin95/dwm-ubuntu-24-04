PROMPT='%{$fg[cyan]%}┌─%{$reset_color%}'
PROMPT+='$(virtualenv_prompt_info)'
PROMPT+="%{$fg[cyan]%}%1~%{$reset_color%}"
PROMPT+=' $(git_prompt_info)'
PROMPT+=$'\n'
PROMPT+='%{$fg[cyan]%}└─%{$reset_color%}'
PROMPT+="%(?:%{$fg_bold[green]%}%1{➜%} :%{$fg_bold[red]%}%1{➜%} )%{$reset_color%}"
RPROMPT='%{$fg[yellow]%}${_cmd_time}%{$reset_color%} %{$fg[cyan]%}%~%{$reset_color%}'


ZSH_THEME_VIRTUALENV_PREFIX="%{$fg[yellow]%}("
ZSH_THEME_VIRTUALENV_SUFFIX=")%{$reset_color%} "

ZSH_THEME_GIT_PROMPT_PREFIX="%{$fg_bold[blue]%}(%{$fg[red]%}"
ZSH_THEME_GIT_PROMPT_SUFFIX="%{$reset_color%} "
ZSH_THEME_GIT_PROMPT_DIRTY="%{$fg[blue]%}) %{$fg[yellow]%}%1{✗%}"
ZSH_THEME_GIT_PROMPT_CLEAN="%{$fg[blue]%})"

zmodload zsh/datetime

_timer_start() { _cmd_start=$EPOCHREALTIME }
_timer_stop() {
  if (( _cmd_start )); then
    local elapsed=$(( EPOCHREALTIME - _cmd_start ))
    _cmd_start=0
    if (( elapsed >= 60 )); then
      _cmd_time=$(printf '%dm%02ds' $((elapsed/60)) $((elapsed%60)))
    elif (( elapsed >= 2 )); then
      _cmd_time=$(printf '%.1fs' $elapsed)
    else
      _cmd_time=""
    fi
  fi
}
autoload -U add-zsh-hook
add-zsh-hook preexec _timer_start
add-zsh-hook precmd  _timer_stop



