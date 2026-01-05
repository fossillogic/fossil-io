/**
 * -----------------------------------------------------------------------------
 * Project: Fossil Logic
 *
 * This file is part of the Fossil Logic project, which aims to develop
 * high-performance, cross-platform applications and libraries. The code
 * contained herein is licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * Author: Michael Gene Brockus (Dreamer)
 * Date: 04/05/2014
 *
 * Copyright (C) 2014-2025 Fossil Logic. All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "fossil/io/soap.h"
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <time.h>
#include <stdio.h>

// LOOKUP MAP

typedef struct { const char *pattern; } pattern_t;

static const pattern_t spam_patterns[] = {
    { "buy now" }, { "click here" }, { "free gift" }, { "subscribe" },
    { "limited offer" }, { "act now" }, { "instant access" },
    { "order today" }, { "exclusive deal" }, { "don't miss out" },
    { "special promotion" }, { "save big" }, { "offer ends soon" },
    { "get your free" }, { "risk free" }, { "join now" }, { "bonus" },
    { "money back" }, { "claim your prize" }, { "hot deal" },
    { "best price" }, { "lowest price" }, { "order now" }, { "urgent" },
    { "today only" }, { "final chance" }, { "exclusive access" },
    { "secret deal" }, { "free trial" }, { "sign up today" }, { "claim now" },
    { "free shipping" }, { "mega discount" }, { "limited stock" }, { "special bonus" },
    { "cash reward" }, { "earn money" }, { "work from home" }, { "get rich" },
    { "fast cash" }, { "profit now" }, { "exclusive membership" }, { "offer expires" },
    { "exclusive content" }, { "free download" }, { "instant win" }, { "prize inside" },
    { "register free" }, { "money saving" }, { "exclusive coupon" }, { "one-time offer" },
    { "bonus gift" }, { "super deal" }, { "limited edition" }, { "exclusive bonus" },
    { "big savings" }, { "act fast" }, { "click to claim" }, { "get access" },
    { "amazing offer" }, { "deal alert" }, { "hot offer" }, { "free voucher" },
    { "apply now" }, { "exclusive promotion" }, { "today's deal" }, { "instant offer" },
    { "urgent offer" }, { "reward points" }, { "exclusive giveaway" }, { "free subscription" },
    { "unlock now" }, { "best deal" }, { "fast offer" }, { "money making" },
    { "limited opportunity" }, { "special access" }, { "claim gift" }, { "instant reward" },
    { "exclusive deal today" }, { "cash bonus" }, { "bonus reward" }, { "top offer" },
    { "mega offer" }, { "discount coupon" }, { "free trial offer" }, { "secret offer" },
    { "one day only" }, { "hurry" }, { "final offer" }, { "special discount" },
    { "cash prize" }, { "exclusive chance" }, { "register now" }, { "big bonus" },
    { "limited availability" }, { "exclusive access code" }, { "free access" }, { "hot sale" },
    { "get it now" }, { "special offer code" }, { "exclusive bonus deal" }, { "fast track" },
    { "instant access code" }, { "limited edition gift" }, { "exclusive prize" }, { "amazing deal" },
    { "cash back" }, { "super offer" }, { "exclusive membership access" }, { "hot discount" },
    { "grab now" }, { "limited promotion" }, { "today only offer" }, { "exclusive opportunity" },
    { "special reward" }, { "claim your bonus" }, { "urgent deal" }, { "biggest discount" },
    { "free gift card" }, { "final chance offer" }, { "exclusive voucher" }, { "super discount" },
    { NULL }
};

static const pattern_t ragebait_patterns[] = {
    { "you won't believe" }, { "shocking" }, { "outrageous" }, { "unbelievable" }, { "infuriating" },
    { "angry" }, { "furious" }, { "disgusting" }, { "ridiculous" }, { "scandal" },
    { "horrifying" }, { "outrage" }, { "outraged" }, { "triggered" }, { "furor" },
    { "angry reaction" }, { "outrageous claim" }, { "outrageous story" }, { "unacceptable" },
    { "outrageous behavior" }, { "shocking truth" }, { "unbelievable act" }, { "inexcusable" },
    { "angry mob" }, { "furious debate" }, { "disgusting act" }, { "ridiculous claim" }, { "shocking event" },
    { "infuriating story" }, { "outrageous incident" }, { "unbelievable event" }, { "horrifying act" },
    { "furious reaction" }, { "outrageous remark" }, { "triggering statement" }, { "infuriating post" },
    { "scandalous behavior" }, { "outraged citizens" }, { "outrageous opinion" }, { "disgusting statement" },
    { "ridiculous act" }, { "unbelievable behavior" }, { "shocking revelation" }, { "unacceptable act" },
    { "outrageous attack" }, { "furious argument" }, { "angry post" }, { "infuriating video" }, { "outrageous content" },
    { "ridiculous claim" }, { "horrifying story" }, { "triggered audience" }, { "outrageous rant" },
    { "furious post" }, { "disgusting action" }, { "outrageous content" }, { "shocking message" },
    { "angry rant" }, { "outrageous content" }, { "ridiculous argument" }, { "infuriating opinion" },
    { "unbelievable statement" }, { "horrifying content" }, { "outraged viewers" }, { "furious commentary" },
    { "triggered audience" }, { "scandalous statement" }, { "outrageous post" }, { "infuriating message" },
    { "ridiculous video" }, { "disgusting content" }, { "unacceptable behavior" }, { "outrageous video" },
    { "angry commentary" }, { "furious reaction" }, { "shocking opinion" }, { "triggered post" }, { "outrageous action" },
    { "ridiculous message" }, { "infuriating video" }, { "furious statement" }, { "outrageous comment" },
    { "shocking post" }, { "angry message" }, { "unbelievable content" }, { "disgusting video" }, { "outrageous tweet" },
    { "ridiculous commentary" }, { "furious post" }, { "infuriating action" }, { "outrageous tweet" },
    { "horrifying video" }, { "triggered comment" }, { "outrageous content" }, { "shocking message" },
    { "unbelievable tweet" }, { "furious comment" }, { "angry post" }, { "outrageous content" },
    { "ridiculous content" }, { "infuriating post" }, { "triggered tweet" }, { NULL }
};

static const pattern_t clickbait_patterns[] = {
    { "you won't believe" }, { "this is why" }, { "what happens next" },
    { "the reason is" }, { "will shock you" }, { "revealed" }, { "top 10" },
    { "number one" }, { "amazing" }, { "secret" }, { "never guess" }, { "incredible" },
    { "shocking" }, { "see why" }, { "you must know" }, { "don't miss" }, { "unbelievable" },
    { "mind blown" }, { "this trick" }, { "life hack" }, { "what they don't want you to know" },
    { "you need this" }, { "before you die" }, { "must see" }, { "everyone is talking about" },
    { "game changer" }, { "epic" }, { "surprising" }, { "this works" }, { "watch this" },
    { "amazing fact" }, { "shocking truth" }, { "revealing" }, { "you'll never guess" },
    { "crazy" }, { "you have to see" }, { "hidden" }, { "astonishing" }, { "exclusive" },
    { "catch this" }, { "once in a lifetime" }, { "incredible story" }, { "jaw dropping" },
    { "life changing" }, { "you won't believe what happened" }, { "secret revealed" },
    { "top secret" }, { "must know" }, { "before it's too late" }, { "what happens when" },
    { "amazing discovery" }, { "unseen" }, { "mind blowing fact" }, { "shocking event" },
    { "this will blow your mind" }, { "what happened next will shock you" }, { "the truth revealed" },
    { "you'll be surprised" }, { "find out why" }, { "the shocking reason" }, { "the real story" },
    { "the truth behind" }, { "you can't miss this" }, { "must watch" }, { "the ultimate guide" },
    { "the best ever" }, { "the most amazing" }, { "the secret to" }, { "the real reason" },
    { "the shocking secret" }, { "the truth about" }, { "the one trick" }, { "the only way" },
    { "the best kept secret" }, { "the shocking truth" }, { "the untold story" }, { "the hidden truth" },
    { "the secret nobody tells you" }, { "the secret everyone is talking about" },
    { "the secret to success" }, { "the secret revealed" }, { "the shocking discovery" },
    { "the secret ingredient" }, { "the secret formula" }, { "the secret weapon" },
    { "the secret behind the success" }, { "the shocking confession" }, { "the shocking admission" },
    { "the shocking revelation" }, { "the shocking expose" }, { "the shocking expose revealed" },
    { NULL }
};

static const pattern_t bot_patterns[] = {
    { "buy now" }, { "click here" }, { "subscribe" }, { "free gift" },
    { "limited offer" }, { "act now" }, { "instant access" }, { "order today" },
    { "exclusive deal" }, { "don't miss out" }, { "special promotion" }, { "save big" },
    { "offer ends soon" }, { "get your free" }, { "risk free" }, { "join now" },
    { "bonus" }, { "money back" }, { "claim your prize" }, { "hot deal" },
    { "best price" }, { "lowest price" }, { "order now" }, { "urgent" },
    { "today only" }, { "final chance" }, { "exclusive access" }, { "secret deal" },
    { "free trial" }, { "sign up today" }, { "claim now" }, { "free shipping" },
    { "register free" }, { "money saving" }, { "exclusive coupon" }, { "one-time offer" },
    { "bonus gift" }, { "super deal" }, { "limited edition" }, { "exclusive bonus" },
    { "big savings" }, { "act fast" }, { "click to claim" }, { "get access" },
    { "amazing offer" }, { "deal alert" }, { "hot offer" }, { "free voucher" },
    { "apply now" }, { "exclusive promotion" }, { "today's deal" }, { "instant offer" },
    { "urgent offer" }, { "reward points" }, { "exclusive giveaway" }, { "free subscription" },
    { "unlock now" }, { "best deal" }, { "fast offer" }, { "money making" },
    { "limited opportunity" }, { "special access" }, { "claim gift" }, { "instant reward" },
    { "exclusive deal today" }, { "cash bonus" }, { "bonus reward" }, { "top offer" },
    { "mega offer" }, { "discount coupon" }, { "free trial offer" }, { "secret offer" },
    { "one day only" }, { "hurry" }, { "final offer" }, { "special discount" },
    { "cash prize" }, { "exclusive chance" }, { "register now" }, { "big bonus" },
    { "limited availability" }, { "exclusive access code" }, { "free access" }, { "hot sale" },
    { "get it now" }, { "special offer code" }, { "exclusive bonus deal" }, { "fast track" },
    { "instant access code" }, { "limited edition gift" }, { "exclusive prize" }, { "amazing deal" },
    { "cash back" }, { "super offer" }, { "exclusive membership access" }, { "hot discount" },
    { "grab now" }, { "limited promotion" }, { "today only offer" }, { "exclusive opportunity" },
    { "special reward" }, { "claim your bonus" }, { "urgent deal" }, { "biggest discount" },
    { "free gift card" }, { "final chance offer" }, { "exclusive voucher" }, { "super discount" },
    { NULL }
};

static const pattern_t marketing_patterns[] = {
    { "limited time" }, { "special offer" }, { "act now" }, { "exclusive" },
    { "sale ends soon" }, { "best deal" }, { "discount" }, { "save now" },
    { "offer expires" }, { "today only" }, { "final chance" }, { "bonus gift" },
    { "unlock savings" }, { "order today" }, { "get it now" }, { "exclusive access" },
    { "mega discount" }, { "special promotion" }, { "cash reward" }, { "register now" },
    { "limited time offer" }, { "exclusive deal" }, { "save big" }, { "lowest price" },
    { "free shipping" }, { "money back" }, { "risk free" }, { "bonus" },
    { "exclusive coupon" }, { "super deal" }, { "limited edition" }, { "exclusive bonus" },
    { "big savings" }, { "act fast" }, { "click to claim" }, { "get access" },
    { "amazing offer" }, { "deal alert" }, { "hot offer" }, { "free voucher" },
    { "apply now" }, { "exclusive promotion" }, { "today's deal" }, { "instant offer" },
    { "urgent offer" }, { "reward points" }, { "exclusive giveaway" }, { "free subscription" },
    { "unlock now" }, { "fast offer" }, { "money making" }, { "limited opportunity" },
    { "special access" }, { "claim gift" }, { "instant reward" }, { "exclusive deal today" },
    { "cash bonus" }, { "bonus reward" }, { "top offer" }, { "mega offer" },
    { "discount coupon" }, { "free trial offer" }, { "secret offer" }, { "one day only" },
    { "hurry" }, { "final offer" }, { "special discount" }, { "cash prize" },
    { "exclusive chance" }, { "big bonus" }, { "limited availability" }, { "exclusive access code" },
    { "free access" }, { "hot sale" }, { "special offer code" }, { "exclusive bonus deal" },
    { "fast track" }, { "instant access code" }, { "limited edition gift" }, { "exclusive prize" },
    { "amazing deal" }, { "cash back" }, { "super offer" }, { "exclusive membership access" },
    { "hot discount" }, { "grab now" }, { "limited promotion" }, { "today only offer" },
    { "exclusive opportunity" }, { "special reward" }, { "claim your bonus" }, { "urgent deal" },
    { "biggest discount" }, { "free gift card" }, { "final chance offer" }, { "exclusive voucher" },
    { "super discount" }, { NULL }
};

static const pattern_t technobabble_patterns[] = {
    { "quantum entanglement" }, { "machine learning" }, { "deep neural network" },
    { "blockchain" }, { "AI-driven" }, { "cloud computing" }, { "hyperconverged" },
    { "cyber-physical" }, { "nanotechnology" }, { "augmented reality" }, { "edge computing" },
    { "IoT-enabled" }, { "autonomous system" }, { "next-gen" }, { "distributed ledger" },
    { "big data" }, { "artificial intelligence" }, { "neural net" }, { "virtual reality" },
    { "smart contract" }, { "tokenization" }, { "decentralized" }, { "cryptocurrency" },
    { "internet of things" }, { "5G" }, { "metaverse" }, { "digital twin" },
    { "self-driving" }, { "robotics" }, { "bioinformatics" }, { "genomics" },
    { "quantum computing" }, { "blockchain technology" }, { "AI-powered" }, { "data lake" },
    { "serverless" }, { "microservices" }, { "containerization" }, { "orchestration" },
    { "edge AI" }, { "fog computing" }, { "digital transformation" }, { "smart city" },
    { "predictive analytics" }, { "prescriptive analytics" }, { "data mining" },
    { "data science" }, { "data engineering" }, { "cloud-native" }, { "multi-cloud" },
    { "hybrid cloud" }, { "zero trust" }, { "blockchain-enabled" }, { "AI-first" },
    { NULL }
};

static const pattern_t hype_patterns[] = {
    { "amazing" }, { "incredible" }, { "epic" }, { "unbelievable" }, { "mind-blowing" },
    { "groundbreaking" }, { "revolutionary" }, { "next level" }, { "must see" },
    { "once in a lifetime" }, { "life changing" }, { "game changer" }, { "exclusive" },
    { "jaw dropping" }, { "life altering" }, { "legendary" }, { "phenomenal" },
    { "unprecedented" }, { "record-breaking" }, { "world-class" }, { "state-of-the-art" },
    { "cutting edge" }, { "breakthrough" }, { "unmatched" }, { "unrivaled" },
    { "unparalleled" }, { "best ever" }, { "never before" }, { "must have" },
    { "can't miss" }, { "top rated" }, { "blockbuster" }, { "sensational" },
    { "spectacular" }, { "mind blowing" }, { "outstanding" }, { "astounding" },
    { "stunning" }, { "breathtaking" }, { "extraordinary" }, { "remarkable" },
    { "unreal" }, { "fantastic" }, { "superb" }, { "magnificent" }, { "marvelous" },
    { NULL }
};

static const pattern_t political_patterns[] = {
    { "vote" }, { "policy" }, { "government" }, { "election" }, { "legislation" },
    { "reform" }, { "candidate" }, { "campaign" }, { "democracy" }, { "party" },
    { "protest" }, { "senate" }, { "congress" }, { "bill" }, { "law" },
    { "parliament" }, { "representative" }, { "constituent" }, { "lobby" },
    { "politician" }, { "political" }, { "governor" }, { "mayor" }, { "president" },
    { "prime minister" }, { "cabinet" }, { "minister" }, { "secretary" },
    { "referendum" }, { "ballot" }, { "poll" }, { "platform" }, { "manifesto" },
    { "coalition" }, { "majority" }, { "minority" }, { "opposition" }, { "incumbent" },
    { "constituency" }, { "district" }, { "jurisdiction" }, { "executive" },
    { "judiciary" }, { "legislature" }, { "bureaucracy" }, { "regulation" },
    { "statute" }, { "ordinance" }, { "decree" }, { "mandate" }, { "impeachment" },
    { NULL }
};

static const pattern_t offensive_patterns[] = {
    { "idiot" }, { "stupid" }, { "dumb" }, { "fool" }, { "loser" },
    { "moron" }, { "hate" }, { "jerk" }, { "trash" }, { "scum" },
    { "nonsense" }, { "pathetic" }, { "worthless" }, { "disgusting" },
    { "ignorant" }, { "cretin" }, { "imbecile" }, { "retard" }, { "sucks" },
    { "garbage" }, { "ugly" }, { "fat" }, { "gross" }, { "stinks" },
    { "douche" }, { "bastard" }, { "asshole" }, { "prick" }, { "bitch" },
    { "slut" }, { "whore" }, { "dick" }, { "piss off" }, { "shut up" },
    { "kill yourself" }, { "kys" }, { "die" }, { "go to hell" }, { "freak" },
    { "weirdo" }, { "psycho" }, { "lunatic" }, { "nutcase" }, { "twat" },
    { "cunt" }, { "fuck" }, { "shit" }, { "bullshit" }, { "crap" },
    { NULL }
};

static const pattern_t misinformation_patterns[] = {
    { "fake news" }, { "hoax" }, { "false" }, { "misleading" }, { "rumor" },
    { "conspiracy" }, { "unverified" }, { "scam" }, { "fraud" }, { "deceptive" },
    { "disinformation" }, { "fabricated" }, { "untrue" }, { "debunked" }, { "myth" },
    { "urban legend" }, { "clickbait" }, { "sensationalized" }, { "out of context" },
    { "doctored" }, { "forged" }, { "faked" }, { "manipulated" }, { "deepfake" },
    { "false claim" }, { "false narrative" }, { "false report" }, { "false story" },
    { "false information" }, { "false statement" }, { "falsehood" }, { "misreport" },
    { "misquote" }, { "misattribute" }, { "misrepresent" }, { "misstate" }, { "misinform" },
    { "mislead" }, { "misinterpret" }, { "misconstrue" }, { "miscommunicate" },
    { NULL }
};

static const pattern_t conspiracy_patterns[] = {
    { "hidden agenda" }, { "secret plan" }, { "shadow government" }, { "cover-up" },
    { "inside job" }, { "elite" }, { "controlled opposition" }, { "false flag" },
    { "government plot" }, { "manipulation" }, { "deep state" }, { "puppet master" },
    { "new world order" }, { "illuminati" }, { "mind control" }, { "chemtrails" },
    { "aliens" }, { "ufo" }, { "area 51" }, { "moon landing hoax" }, { "flat earth" },
    { "reptilian" }, { "secret society" }, { "big pharma" }, { "big tech" },
    { "big oil" }, { "big brother" }, { "mass surveillance" }, { "plandemic" },
    { "crisis actor" }, { "hoax" }, { "cover up" }, { "deep cover" }, { "psyop" },
    { "psy ops" }, { "psy-ops" }, { "psy ops operation" }, { "psy-ops operation" },
    { "psy ops campaign" }, { "psy-ops campaign" }, { "psy ops program" }, { "psy-ops program" },
    { NULL }
};

static const pattern_t formal_patterns[] = {
    { "therefore" }, { "moreover" }, { "hence" }, { "thus" }, { "accordingly" },
    { "in conclusion" }, { "furthermore" }, { "consequently" }, { "as a result" }, { "in addition" },
    { "notwithstanding" }, { "nevertheless" }, { "nonetheless" }, { "whereas" }, { "hereby" },
    { "herein" }, { "herewith" }, { "heretofore" }, { "therein" }, { "therewith" },
    { "thereafter" }, { "thereupon" }, { "thereby" }, { "wherein" }, { "whereof" },
    { "whereupon" }, { "whereby" }, { "pursuant to" }, { "in accordance with" },
    { "insofar as" }, { "inasmuch as" }, { "in the event that" }, { "in the absence of" },
    { "in the presence of" }, { "in the course of" }, { "in the context of" },
    { "in the light of" }, { "in the interest of" }, { "in the process of" },
    { "in the case of" }, { "in the matter of" }, { "in the opinion of" },
    { NULL }
};

static const pattern_t casual_patterns[] = {
    { "hey" }, { "lol" }, { "omg" }, { "btw" }, { "idk" },
    { "yep" }, { "nah" }, { "cool" }, { "awesome" }, { "what's up" },
    { "sup" }, { "yo" }, { "dude" }, { "bro" }, { "fam" }, { "bruh" },
    { "lmao" }, { "rofl" }, { "haha" }, { "hehe" }, { "xd" }, { "ikr" },
    { "tbh" }, { "fyi" }, { "imo" }, { "imho" }, { "smh" }, { "fml" },
    { "ily" }, { "nope" }, { "yolo" }, { "meh" }, { "ugh" }, { "yay" },
    { "yesss" }, { "noooo" }, { "yaaaas" }, { "bruh moment" }, { "oh no" },
    { "omfg" }, { "wtf" }, { "wtfv" }, { "lmaoed" }, { "lmaoing" },
    { "haha lol" }, { "lol haha" }, { "xd lol" }, { "lol xd" }, { "fml lol" },
    { "smdh" }, { "sksksk" }, { "and i oop" }, { "yeet" }, { "yeeted" },
    { "yeeting" }, { "uwu" }, { "owo" }, { "rawr" }, { "rawr x3" },
    { NULL }
};

static const pattern_t sarcasm_patterns[] = {
    { "yeah right" }, { "as if" }, { "sure" }, { "oh great" }, { "fantastic" },
    { "brilliant" }, { "wow" }, { "amazing" }, { "just perfect" }, { "totally" },
    { "obviously" }, { "of course" }, { "nice job" }, { "good luck with that" },
    { "what a surprise" }, { "how original" }, { "so helpful" }, { "thanks a lot" },
    { "just what I needed" }, { "couldn't be better" }, { "love it" }, { "can't wait" },
    { "so excited" }, { "best ever" }, { "what a genius" }, { "so smart" },
    { "how clever" }, { "so funny" }, { "hilarious" }, { "great idea" },
    { NULL }
};

static const pattern_t neutral_patterns[] = {
    { "okay" }, { "alright" }, { "yes" }, { "no" }, { "maybe" },
    { "fine" }, { "good" }, { "bad" }, { "average" }, { "normal" },
    { "regular" }, { "standard" }, { "typical" }, { "usual" }, { "ordinary" },
    { "so-so" }, { "meh" }, { "not bad" }, { "not good" }, { "could be better" },
    { "could be worse" }, { "nothing special" }, { "not sure" }, { "not certain" },
    { "uncertain" }, { "possibly" }, { "perhaps" }, { "potentially" }, { "likely" },
    { "unlikely" }, { "neutral" }, { "indifferent" }, { "unbiased" }, { "impartial" },
    { NULL }
};

static const pattern_t aggressive_patterns[] = {
    { "attack" }, { "destroy" }, { "fight" }, { "kill" }, { "smash" },
    { "crush" }, { "annihilate" }, { "rage" }, { "strike" }, { "obliterate" },
    { "dominate" }, { "conquer" }, { "invade" }, { "overpower" }, { "subdue" },
    { "eliminate" }, { "eradicate" }, { "wipe out" }, { "take down" }, { "beat" },
    { "defeat" }, { "punish" }, { "hurt" }, { "harm" }, { "maul" }, { "batter" },
    { "brawl" }, { "clash" }, { "assault" }, { "barrage" }, { "bombard" },
    { "ambush" }, { "charge" }, { "storm" }, { "bust" }, { "wreck" },
    { "wreck havoc" }, { "tear apart" }, { "rip apart" }, { "rip to shreds" },
    { NULL }
};

static const pattern_t emotional_patterns[] = {
    { "happy" }, { "sad" }, { "angry" }, { "excited" }, { "fear" },
    { "joy" }, { "love" }, { "hate" }, { "surprised" }, { "ecstatic" },
    { "depressed" }, { "anxious" }, { "nervous" }, { "thrilled" }, { "delighted" },
    { "miserable" }, { "furious" }, { "elated" }, { "content" }, { "disappointed" },
    { "frustrated" }, { "overjoyed" }, { "heartbroken" }, { "grateful" }, { "resentful" },
    { "jealous" }, { "envious" }, { "ashamed" }, { "proud" }, { "guilty" },
    { "hopeful" }, { "hopeless" }, { "relieved" }, { "stressed" }, { "calm" },
    { "peaceful" }, { "confused" }, { "bored" }, { "lonely" }, { "scared" },
    { NULL }
};

static const pattern_t passive_aggressive_patterns[] = {
    { "sure" }, { "whatever" }, { "fine" }, { "if you say so" }, { "okay then" },
    { "no problem" }, { "as you wish" }, { "right" }, { "yeah, sure" },
    { "if that's what you want" }, { "I guess" }, { "if you insist" },
    { "not my problem" }, { "do what you want" }, { "go ahead" }, { "be my guest" },
    { "I don't care" }, { "it's up to you" }, { "suit yourself" }, { "have it your way" },
    { "whatever you say" }, { "if it makes you happy" }, { "I'm fine" },
    { "I'm okay" }, { "I'm good" }, { "it's fine" }, { "it's okay" },
    { "it's whatever" }, { "that's fine" }, { "that's okay" }, { "that's whatever" },
    { NULL }
};

static const pattern_t snowflake_patterns[] = {
    { "triggered" }, { "microaggression" }, { "safe space" }, { "privilege" },
    { "problematic" }, { "offended" }, { "snowflake" }, { "fragile" },
    { "woke" }, { "cancel culture" }, { "toxic" }, { "gaslighting" },
    { "trauma" }, { "oppressed" }, { "victim" }, { "identity politics" },
    { "intersectional" }, { "systemic" }, { "inclusion" }, { "diversity" },
    { "equity" }, { "ally" }, { "marginalized" }, { "lived experience" },
    { "call out" }, { "callout" }, { "safe zone" }, { "emotional labor" },
    { "tone policing" }, { "cultural appropriation" }, { "trigger warning" },
    { "unpack" }, { "problematic fave" }, { "problematic favorite" },
    { "privileged" }, { "decolonize" }, { "inclusivity" }, { "oppression" },
    { "systemic bias" }, { "implicit bias" }, { "white fragility" },
    { "white privilege" }, { "check your privilege" }, { "lived experiences" },
    { "identity" }, { "safe environment" }, { "emotional support" },
    { "emotional damage" }, { "emotional distress" }, { "emotional safety" },
    { "emotional wellbeing" }, { "emotional well-being" }, { "emotional health" },
    { "emotional needs" }, { "emotional response" }, { "emotional trauma" },
    { "emotional abuse" }, { "emotional burden" }, { "emotional exhaustion" },
    { "emotional intelligence" }, { "emotional resilience" }, { "emotional sensitivity" },
    { "emotional support animal" }, { "emotional trigger" }, { "emotional vulnerability" },
    { NULL }
};

static const pattern_t brain_rot_patterns[] = {
    {"lol"}, {"lmao"}, {"rofl"}, {"bruh"}, {"wtf"}, {"omg"}, {"haha"}, {"hehe"}, {"xd"}, {"xdxd"},
    {"yo"}, {"hey"}, {"sup"}, {"what's up"}, {"man"}, {"dude"}, {"bro"}, {"broski"}, {"homie"}, {"fam"},
    {"yolo"}, {"smh"}, {"fml"}, {"idk"}, {"ikr"}, {"tbh"}, {"ily"}, {"ily2"}, {"omfg"}, {"wtfv"},
    {"heh"}, {"meh"}, {"ugh"}, {"aww"}, {"yay"}, {"yayyy"}, {"yesss"}, {"nope"}, {"nah"}, {"yep"},
    {"bruhh"}, {"brooo"}, {"duuuude"}, {"lolz"}, {"lols"}, {"lul"}, {"lulz"}, {"hahaaha"}, {"roflmao"}, {"lmfao"},
    {"kek"}, {"pog"}, {"poggers"}, {"pogchamp"}, {"rip"}, {"gg"}, {"ggwp"}, {"rekt"}, {"owned"}, {"clap"},
    {"clapclap"}, {"facepalm"}, {"yikes"}, {"oops"}, {"oopsie"}, {"ayyy"}, {"ayyy lmao"}, {"hehehe"}, {"yayyyyy"}, {"nooooo"},
    {"yaaaas"}, {"bruh moment"}, {"oh no"}, {"ikr"}, {"omgosh"}, {"lmaoed"}, {"lmaoing"}, {"haha lol"}, {"lol haha"}, {"xd lol"},
    {"lol xd"}, {"fml lol"}, {"smdh"}, {"sksksk"}, {"and i oop"}, {"yeet"}, {"yeeted"}, {"yeeting"}, {"uwu"}, {"owo"},
    {"uwu uwu"}, {"owo owo"}, {"rawr"}, {"rawr x3"}, {">:("}, {"<3"}, {":3"}, {"-_-"}, {"^_^"}, {"T_T"},
    {";-;"}, {">:O"}, {"-.-"}, {">:("}, {"D:"}, {"XD"}, {"xD"}, {"XD XD"}, {">:|"}, {"-.-'"},
    {"-__-"}, {"o.O"}, {"O.o"}, {"lolwut"}, {"lold"}, {"lolol"}, {"lololol"}, {"haha lmao"}, {"roflol"}, {"roflolmao"},
    {"wtf lol"}, {"no cap"}, {"sus"}, {"sussy"}, {"sus af"}, {"based"}, {"cringe"}, {"bet"}, {"lit"}, {"fire"},
    {"goat"}, {"slay"}, {"flex"}, {"vibe"}, {"vibing"}, {"mood"}, {"big mood"}, {"dead"}, {"i'm dead"}, {"rip me"},
    {"lowkey"}, {"highkey"}, {"salty"}, {"thirsty"}, {"shade"}, {"shook"}, {"stan"}, {"simp"}, {"snacc"}, {"snack"},
    {"bop"}, {"bussin"}, {"drip"}, {"drippy"}, {"fam jam"}, {"fam squad"}, {"fam bam"}, {"fam fam"}, {"famalam"}, {"fam squad"},
    {"on fleek"}, {"fleek"}, {"savage"}, {"savage af"}, {"extra"}, {"basic"}, {"woke"}, {"cancelled"}, {"cancel"}, {"clout"},
    {"clout chaser"}, {"receipts"}, {"tea"}, {"spill the tea"}, {"spill tea"}, {"big yikes"}, {"big oof"}, {"oof"}, {"oop"},
    {"no chill"}, {"deadass"}, {"facts"}, {"big facts"}, {"periodt"}, {"period"}, {"sis"}, {"sis bro"}, {"sis fam"}, {"sis queen"},
    {NULL}
};

/* ============================================================================
 * Internal helpers
 * ============================================================================ */

static char *dupstr(const char *s) {
    if (!s) return NULL;
    size_t n = strlen(s);
    char *r = (char*)malloc(n + 1);
    if (r) memcpy(r, s, n + 1);
    return r;
}

static void strtolower(char *s) {
    if (!s) return;
    for (; *s; s++) *s = (char)tolower((unsigned char)*s);
}

static int soap_is_abbrev(const char *s) {
    static const char *abbr[] = {
        "mr.", "mrs.", "dr.", "vs.", "etc.", "e.g.", "i.e.", NULL
    };
    for (int i = 0; abbr[i]; i++)
        if (strcasecmp(s, abbr[i]) == 0)
            return 1;
    return 0;
}

/* ============================================================================
 * Leetspeak normalization
 * ============================================================================ */

/*
 * Advanced leetspeak normalization:
 * Handles common single-char and multi-char leet patterns, e.g. "|3"->"b", "ph"->"f", "|-|"=>"h", etc.
 * Converts leet to closest ASCII letter, in-place.
 */
static char leet_map_adv(const char *s, int *consumed) {
    // Multi-char patterns (longest first)
    struct { const char *leet; char plain; } table[] = {
        {"/\\", 'a'}, {"|\\|", 'n'}, {"/\\/", 'n'}, {"|-|", 'h'}, {"|3", 'b'}, {"13", 'b'},
        {"ph", 'f'}, {"|2", 'r'}, {"][", 'h'}, {"()", 'o'}, {"|_", 'l'}, {"|<", 'k'},
        {"|>", 'p'}, {"|*", 'p'}, {"|o", 'p'}, {"[]", 'o'}, {"_|", 'j'}, {"|{", 'k'},
        {"|/", 'j'}, {"|_|", 'u'}, {"/\\/\\", 'm'}, {"|\\/|", 'm'}, {"\\/\\/", 'w'},
        {"vv", 'w'}, {"\\\\//", 'v'}, {"%", 'x'}, {"><", 'x'}, {"7_", 'z'}, {NULL, 0}
    };
    for (int i = 0; table[i].leet; i++) {
        size_t n = strlen(table[i].leet);
        if (strncmp(s, table[i].leet, n) == 0) {
            if (consumed) *consumed = (int)n;
            return table[i].plain;
        }
    }
    // Single-char patterns
    switch (*s) {
        case '4': case '@': return 'a';
        case '8': return 'b';
        case '(': case '<': case '{': case '[': return 'c';
        case '3': return 'e';
        case '6': case '9': return 'g';
        case '#': return 'h';
        case '1': case '!': case '|': return 'i';
        case '0': return 'o';
        case '$': case '5': return 's';
        case '7': return 't';
        case '2': return 'z';
        default: return *s;
    }
}

static void normalize_leet(char *s) {
    if (!s) return;
    char *src = s, *dst = s;
    while (*src) {
        int consumed = 1;
        char c = leet_map_adv(src, &consumed);
        *dst++ = c;
        src += consumed;
    }
    *dst = 0;
}

/* ============================================================================
 * Morse decoding (basic)
 * ============================================================================ */

typedef struct { const char *code; char c; } morse_t;

static const morse_t morse_table[] = {
    {".-", 'a'}, {"-...", 'b'}, {"-.-.", 'c'}, {"-..", 'd'},
    {".", 'e'}, {"..-.", 'f'}, {"--.", 'g'}, {"....", 'h'},
    {"..", 'i'}, {".---", 'j'}, {"-.-", 'k'}, {".-..", 'l'},
    {"--", 'm'}, {"-.", 'n'}, {"---", 'o'}, {".--.", 'p'},
    {"--.-", 'q'}, {".-.", 'r'}, {"...", 's'}, {"-", 't'},
    {"..-", 'u'}, {"...-", 'v'}, {".--", 'w'}, {"-..-", 'x'},
    {"-.--", 'y'}, {"--..", 'z'},
    {"-----", '0'}, {".----", '1'}, {"..---", '2'}, {"...--", '3'},
    {"....-", '4'}, {".....", '5'}, {"-....", '6'}, {"--...", '7'},
    {"---..", '8'}, {"----.", '9'},
    {".-.-.-", '.'}, {"--..--", ','}, {"..--..", '?'}, {".----.", '\''},
    {"-.-.--", '!'}, {"-..-.", '/'}, {"-.--.", '('}, {"-.--.-", ')'},
    {".-...", '&'}, {"---...", ':'}, {"-.-.-.", ';'}, {"-...-", '='},
    {".-.-.", '+'}, {"-....-", '-'}, {"..--.-", '_'}, {".-..-.", '\"'},
    {".--.-.", '@'},
    {NULL, 0}
};

static char morse_lookup(const char *code) {
    for (int i = 0; morse_table[i].code; i++)
        if (strcmp(morse_table[i].code, code) == 0) return morse_table[i].c;
    return '?';
}

// Advanced Morse decoder: handles multiple spaces as word breaks, ignores invalid chars, trims output
static char *decode_morse(const char *text) {
    if (!text) return NULL;
    size_t len = strlen(text);
    char *out = (char*)malloc(len + 2);
    if (!out) return NULL;
    size_t oi = 0, bi = 0;
    char buf[16];
    int last_was_space = 0;
    for (const char *p = text;; p++) {
        if (*p == '.' || *p == '-') {
            if (bi < sizeof(buf) - 1)
                buf[bi++] = *p;
            last_was_space = 0;
        } else {
            if (bi) {
                buf[bi] = 0;
                out[oi++] = morse_lookup(buf);
                bi = 0;
            }
            if (*p == ' ' || *p == '/' || *p == '\t') {
                // Word break: two or more spaces or slash
                if (last_was_space) {
                    out[oi++] = ' ';
                    last_was_space = 0;
                } else {
                    last_was_space = 1;
                }
            } else {
                last_was_space = 0;
            }
            if (!*p) break;
        }
    }
    // Remove trailing spaces
    while (oi > 0 && out[oi-1] == ' ') oi--;
    out[oi] = 0;
    return out;
}

/* ============================================================================
 * Sanitization
 * ============================================================================ */

char *fossil_io_soap_normalize(const char *text) {
    if (!text) return NULL;
    size_t len = strlen(text);
    if (len == 0) {
        // Return empty string for empty input
        char *empty = (char *)malloc(1);
        if (empty) empty[0] = 0;
        return empty;
    }

    // Remove leading/trailing whitespace, collapse multiple spaces, advanced leet normalization, lowercase
    char *tmp = (char *)malloc(len + 2);
    if (!tmp) return NULL;
    size_t j = 0;
    int last_space = 0;
    // Skip leading whitespace
    size_t i = 0;
    while (isspace((unsigned char)text[i]) && text[i] != '\n') i++;
    for (; text[i]; i++) {
        unsigned char c = (unsigned char)text[i];
        if (c < 32 && c != '\n') continue; // skip control chars except newline
        if (isspace(c) && c != '\n') {
            if (!last_space) tmp[j++] = ' ';
            last_space = 1;
        } else {
            tmp[j++] = text[i];
            last_space = 0;
        }
    }
    // Remove trailing whitespace
    while (j > 0 && isspace((unsigned char)tmp[j-1]) && tmp[j-1] != '\n') j--;
    tmp[j] = 0;

    // Advanced leet normalization (in-place)
    normalize_leet(tmp);

    // Lowercase
    strtolower(tmp);

    // Remove punctuation except for sentence-ending and intra-word apostrophes
    char *out = (char *)malloc(strlen(tmp) + 1);
    if (!out) { free(tmp); return NULL; }
    size_t k = 0;
    for (size_t m = 0; tmp[m]; m++) {
        char c = tmp[m];
        if (isalnum((unsigned char)c) || c == ' ' || c == '\n' ||
            c == '.' || c == '!' || c == '?' ||
            (c == '\'' && m > 0 && isalpha((unsigned char)tmp[m-1]) && isalpha((unsigned char)tmp[m+1]))) {
            out[k++] = c;
        }
    }
    out[k] = 0;
    free(tmp);

    // Remove trailing spaces again (for cases like "Only one sentence. ")
    while (k > 0 && isspace((unsigned char)out[k-1]) && out[k-1] != '\n') out[--k] = 0;

    return out;
}

char *fossil_io_soap_sanitize(const char *text) {
    if (!text) return NULL;
    size_t len = strlen(text);
    if (len == 0) {
        char *empty = (char *)malloc(1);
        if (empty) empty[0] = 0;
        return empty;
    }
    // Remove control chars except newline, trim leading/trailing whitespace, collapse multiple spaces
    char *tmp = (char *)malloc(len + 2);
    if (!tmp) return NULL;
    size_t j = 0;
    int last_space = 0;
    // Skip leading whitespace
    size_t i = 0;
    while (isspace((unsigned char)text[i]) && text[i] != '\n') i++;
    for (; text[i]; i++) {
        unsigned char c = (unsigned char)text[i];
        if (c < 32 && c != '\n') {
            // Replace control chars (except newline) with space
            if (!last_space) tmp[j++] = ' ';
            last_space = 1;
            continue;
        }
        if (isspace(c) && c != '\n') {
            if (!last_space) tmp[j++] = ' ';
            last_space = 1;
        } else {
            tmp[j++] = text[i];
            last_space = 0;
        }
    }
    // Remove trailing whitespace
    while (j > 0 && isspace((unsigned char)tmp[j-1]) && tmp[j-1] != '\n') j--;
    tmp[j] = 0;

    char *norm = fossil_io_soap_normalize(tmp);
    free(tmp);
    return norm;
}

/* ============================================================================
 * Grammar & style analysis
 * ============================================================================ */

fossil_io_soap_grammar_style_t fossil_io_soap_analyze_grammar_style(const char *text) {
    fossil_io_soap_grammar_style_t r = {1, 0, "neutral", 0, 0, NULL, NULL, 100, {0,0,0,0}};
    if (!text) return r;

    int passive = 0, words = 0, grammar_errors = 0, style_inconsistencies = 0;
    const char *style = "neutral";
    int style_confidence = 80;
    const char *grammar_errs[16] = {NULL};
    const char *style_incons[16] = {NULL};
    int grammar_err_idx = 0, style_incons_idx = 0;

    const char *p = text;
    while (*p) {
        // Count words
        if (isspace((unsigned char)*p)) words++;

        // Passive voice: look for "was|were|is|are|been|being" + " " + word ending in "ed"
        if (!strncmp(p, "was ", 4) || !strncmp(p, "were ", 5) ||
            !strncmp(p, "is ", 3) || !strncmp(p, "are ", 4) ||
            !strncmp(p, "been ", 5) || !strncmp(p, "being ", 6)) {
            const char *q = p;
            while (*q && !isspace((unsigned char)*q)) q++;
            while (*q && isspace((unsigned char)*q)) q++;
            // Look for word ending in "ed"
            char buf[32] = {0};
            int i = 0;
            while (*q && isalpha((unsigned char)*q) && i < 30) buf[i++] = *q++;
            buf[i] = 0;
            size_t blen = strlen(buf);
            if (blen > 2 && buf[blen-2] == 'e' && buf[blen-1] == 'd')
                passive++;
        }
        p++;
    }
    if (words) r.passive_voice_pct = (passive * 100) / words;

    // Style detection (improved heuristics)
    int has_exclaim = strstr(text, "!") != NULL;
    int has_question = strstr(text, "?") != NULL;
    int has_semicolon = strstr(text, ";") != NULL;
    int has_colon = strstr(text, ":") != NULL;
    int has_formal = strstr(text, "therefore") || strstr(text, "however") || strstr(text, "thus");
    int has_casual = strstr(text, "lol") || strstr(text, "btw") || strstr(text, "hey");
    int has_emojis = strstr(text, ":)") || strstr(text, ":(") || strstr(text, ";)");
    int has_caps = 0;
    for (const char *q = text; *q; q++) {
        if (isupper((unsigned char)*q)) { has_caps = 1; break; }
    }

    if (has_exclaim || has_question) {
        style = "emotional";
        style_confidence = 70;
    }
    if (has_formal || has_semicolon || has_colon) {
        style = "formal";
        style_confidence = 80;
    }
    if (has_casual || has_emojis) {
        style = "casual";
        style_confidence = 80;
    }
    if (has_caps && !has_formal && !has_casual) {
        style = "emphatic";
        style_confidence = 60;
    }

    // Grammar error: double spaces
    if (strstr(text, "  ")) {
        grammar_errors++;
        grammar_errs[grammar_err_idx++] = "Double space detected";
        r.grammar_ok = 0;
    }
    // Grammar error: repeated punctuation (e.g. "!!", "??", "...")
    if (strstr(text, "!!") || strstr(text, "??") || strstr(text, "...")) {
        grammar_errors++;
        grammar_errs[grammar_err_idx++] = "Repeated punctuation";
        r.grammar_ok = 0;
    }
    // Grammar error: missing terminal punctuation
    size_t len = strlen(text);
    if (len > 0 && !strchr(".!?", text[len-1])) {
        grammar_errors++;
        grammar_errs[grammar_err_idx++] = "Missing terminal punctuation";
        r.grammar_ok = 0;
    }
    // Grammar error: all lowercase sentence start
    if (isalpha((unsigned char)text[0]) && islower((unsigned char)text[0])) {
        grammar_errors++;
        grammar_errs[grammar_err_idx++] = "Sentence does not start with a capital letter";
        r.grammar_ok = 0;
    }

    // Style inconsistency: mix of "!" and "."
    if (has_exclaim && strchr(text, '.')) {
        style_inconsistencies++;
        style_incons[style_incons_idx++] = "Mixed emotional and neutral punctuation";
    }
    // Style inconsistency: mix of formal and casual
    if (has_formal && has_casual) {
        style_inconsistencies++;
        style_incons[style_incons_idx++] = "Mix of formal and casual language";
    }
    // Style inconsistency: excessive capitalization
    if (has_caps && strlen(text) > 10) {
        int cap_count = 0, total = 0;
        for (const char *q = text; *q; q++) {
            if (isalpha((unsigned char)*q)) {
                total++;
                if (isupper((unsigned char)*q)) cap_count++;
            }
        }
        if (total > 0 && cap_count * 2 > total) {
            style_inconsistencies++;
            style_incons[style_incons_idx++] = "Excessive capitalization";
        }
    }

    r.style = style;
    r.style_confidence = style_confidence;
    r.grammar_error_count = grammar_errors;
    r.style_inconsistency_count = style_inconsistencies;
    r.grammar_errors = (grammar_err_idx > 0) ? (const char **)grammar_errs : NULL;
    r.style_inconsistencies = (style_incons_idx > 0) ? (const char **)style_incons : NULL;

    return r;
}

char *fossil_io_soap_correct_grammar(const char *text)
{
    if (!text) return NULL;

    // --- Begin enhanced context-aware grammar correction ---
    size_t outcap = strlen(text) * 2 + 64;
    char *out = malloc(outcap);
    if (!out) return NULL;

    const char *p = text;
    char *q = out;

    int new_sentence = 1;
    int last_space = 0;
    char last_char = 0;

    int in_quote = 0;
    int in_paren = 0;
    int in_url = 0;
    int word_start = 1;

    // For abbreviation detection
    char prev_word[32] = {0};
    int prev_word_len = 0;
    
    int ellipsis = 0;
    int after_punct = 0;

    struct {
        const char *from;
        const char *to;
    } contractions[] = {
        {" dont ", " don't "}, {" cant ", " can't "}, {" wont ", " won't "},
        {" isnt ", " isn't "}, {" arent ", " aren't "}, {" wasnt ", " wasn't "},
        {" werent ", " weren't "}, {" doesnt ", " doesn't "}, {" didnt ", " didn't "},
        {" hasnt ", " hasn't "}, {" havent ", " haven't "}, {" hadnt ", " hadn't "},
        {" couldnt ", " couldn't "}, {" wouldnt ", " wouldn't "}, {" shouldnt ", " shouldn't "},
        {" mustnt ", " mustn't "}, {" neednt ", " needn't "}, {" darent ", " daren't "},
        {" im ", " I'm "}, {" ive ", " I've "}, {" ill ", " I'll "}, {" id ", " I'd "},
        {" youre ", " you're "}, {" youve ", " you've "}, {" youll ", " you'll "},
        {" youd ", " you'd "}, {" hes ", " he's "}, {" hed ", " he'd "}, {" hell ", " he'll "},
        {" shes ", " she's "}, {" shed ", " she'd "}, {" shell ", " she'll "},
        {" its ", " it's "}, {" were ", " we're "}, {" theyre ", " they're "},
        {" theyve ", " they've "}, {" theyll ", " they'll "}, {" theyd ", " they'd "},
        {" thats ", " that's "}, {" theres ", " there's "}, {" whats ", " what's "},
        {" whos ", " who's "}, {" wheres ", " where's "}, {" whens ", " when's "},
        {" whys ", " why's "}, {" hows ", " how's "}, {" couldve ", " could've "},
        {" wouldve ", " would've "}, {" shouldve ", " should've "}, {" mightve ", " might've "},
        {" mustve ", " must've "}, {" mayve ", " may've "}, {" lets ", " let's "},
        {" thats ", " that's "}, {" theres ", " there's "}, {" dont.", " don't."},
        {" dont,", " don't,"}, {" cant.", " can't."}, {" cant,", " can't,"},
        {" wont.", " won't."}, {" wont,", " won't,"}, {NULL, NULL}
    };

    while (*p) {
        char c = *p;

        // Track quote and paren state
        if (c == '"' || c == '\'') in_quote ^= 1;
        if (c == '(') in_paren++;
        if (c == ')') if (in_paren > 0) in_paren--;

        // URL detection (cheap, no regex)
        if (!in_url && (strncmp(p, "http://", 7) == 0 ||
                        strncmp(p, "https://", 8) == 0))
            in_url = 1;
        if (in_url && (isspace((unsigned char)c) || c == '"' || c == '\'')) // end url on space or quote
            in_url = 0;

        // Normalize whitespace (collapse multiple spaces/tabs)
        if (isspace((unsigned char)c)) {
            if (!last_space && !after_punct) {
                *q++ = ' ';
                last_space = 1;
            }
            word_start = 1;
            p++;
            continue;
        }
        last_space = 0;
        after_punct = 0;   // ← RESET HERE

        // Word boundary and sentence word count
        if (isalpha((unsigned char)c) && word_start) {
            word_start = 0;
        }
        if (!isalpha((unsigned char)c))
            word_start = 1;

        // Abbreviation detection (look back for word before '.')
        if (c == '.' && prev_word_len > 0) {
            prev_word[prev_word_len] = 0;
        }

        // Capitalize start of sentences
        int is_abbrev = 0;
        if (new_sentence && isalpha((unsigned char)c)) {
            if (prev_word_len > 0 && soap_is_abbrev(prev_word))
                is_abbrev = 1;
        
            if (!is_abbrev && !ellipsis)
                c = (char)toupper((unsigned char)c);
        
            new_sentence = 0;
            ellipsis = 0;   // ← RESET HERE
        }

        // Punctuation collapse, but smarter
        if (ispunct((unsigned char)c) && c == last_char) {
            // Collapse repeated punctuation except for ellipsis
            if (c == '.' && *(p+1) == '.' && *(p+2) == '.') {
                // allow ellipsis
            } else if (c == '.' || c == '!' || c == '?') {
                p++;
                continue;
            }
        }

        // Sentence boundary detection (punctuation-aware)
        if ((c == '.' || c == '!' || c == '?') && !in_quote) {
            // Decimal number: don't end sentence
            if (c == '.' && isdigit((unsigned char)*(p+1)))
                ;
            // Abbreviation: don't end sentence
            else if (is_abbrev)
                ;
            else if (c == '.' && *(p+1) == '.' && *(p+2) == '.') {
                ellipsis = 1;
            }
            else
                new_sentence = 1;
        }

        // Context-aware contraction replacement (inline, not global)
        int contraction_found = 0;
        if (!in_quote && !in_url && !in_paren) {
            for (int i = 0; contractions[i].from; i++) {
                size_t len = strlen(contractions[i].from);
                if (strncmp(p, contractions[i].from, len) == 0) {
                    memcpy(q, contractions[i].to, strlen(contractions[i].to));
                    q += strlen(contractions[i].to);
                    p += len;
                    last_char = contractions[i].to[strlen(contractions[i].to)-1];
                    contraction_found = 1;
                    break;
                }
            }
        }
        if (contraction_found) continue;

        // Smart spacing after punctuation (ensure single space after .!?)
        if ((c == '.' || c == '!' || c == '?') && !in_quote && !in_url) {
            *q++ = c;
            last_char = c;
            p++;
            // Skip any spaces after punctuation
            while (isspace((unsigned char)*p)) p++;
            // Insert a single space if not end of string or next is punctuation
            if (*p && !ispunct((unsigned char)*p) && *p != '\0') {
                *q++ = ' ';
                last_space = 1;
            }
            after_punct = 1;
            continue;
        }

        // Fix common missing apostrophes in contractions (e.g. dont -> don't)
        int contraction_fixed = 0;
        if (!in_quote && !in_url && isalpha((unsigned char)c)) {
            // Look for common patterns like "dont", "cant", etc.
            if ((p == text || isspace((unsigned char)*(p-1)) || ispunct((unsigned char)*(p-1))) &&
                (strncmp(p, "dont", 4) == 0 || strncmp(p, "cant", 4) == 0 ||
                 strncmp(p, "wont", 4) == 0 || strncmp(p, "isnt", 4) == 0 ||
                 strncmp(p, "arent", 5) == 0 || strncmp(p, "wasnt", 5) == 0 ||
                 strncmp(p, "werent", 6) == 0 || strncmp(p, "doesnt", 6) == 0 ||
                 strncmp(p, "didnt", 5) == 0 || strncmp(p, "hasnt", 5) == 0 ||
                 strncmp(p, "havent", 6) == 0 || strncmp(p, "hadnt", 5) == 0 ||
                 strncmp(p, "couldnt", 7) == 0 || strncmp(p, "wouldnt", 7) == 0 ||
                 strncmp(p, "shouldnt", 8) == 0 || strncmp(p, "mustnt", 6) == 0 ||
                 strncmp(p, "neednt", 6) == 0 || strncmp(p, "darent", 6) == 0)) {
                // Insert apostrophe before last t
                size_t wlen = 0;
                while (isalpha((unsigned char)p[wlen])) wlen++;
                if (wlen > 3 && wlen < 10) {
                    for (size_t i = 0; i < wlen; i++) {
                        if (i == wlen - 3) *q++ = '\'';
                        *q++ = p[i];
                    }
                    p += wlen;
                    last_char = p[-1];
                    contraction_fixed = 1;
                }
            }
        }
        if (contraction_fixed) continue;

        // Expand common informal forms (e.g. "gonna" -> "going to", "wanna" -> "want to")
        int informal_expanded = 0;
        if (!in_quote && !in_url && isalpha((unsigned char)c)) {
            if ((p == text || isspace((unsigned char)*(p-1)) || ispunct((unsigned char)*(p-1)))) {
                if (strncmp(p, "gonna", 5) == 0 && !isalpha((unsigned char)p[5])) {
                    memcpy(q, "going to", 8); q += 8; p += 5; last_char = 'o'; informal_expanded = 1;
                } else if (strncmp(p, "wanna", 5) == 0 && !isalpha((unsigned char)p[5])) {
                    memcpy(q, "want to", 7); q += 7; p += 5; last_char = 'o'; informal_expanded = 1;
                } else if (strncmp(p, "gotta", 5) == 0 && !isalpha((unsigned char)p[5])) {
                    memcpy(q, "got to", 6); q += 6; p += 5; last_char = 'o'; informal_expanded = 1;
                } else if (strncmp(p, "kinda", 5) == 0 && !isalpha((unsigned char)p[5])) {
                    memcpy(q, "kind of", 7); q += 7; p += 5; last_char = 'f'; informal_expanded = 1;
                } else if (strncmp(p, "sorta", 5) == 0 && !isalpha((unsigned char)p[5])) {
                    memcpy(q, "sort of", 7); q += 7; p += 5; last_char = 'f'; informal_expanded = 1;
                } else if (strncmp(p, "outta", 5) == 0 && !isalpha((unsigned char)p[5])) {
                    memcpy(q, "out of", 6); q += 6; p += 5; last_char = 'f'; informal_expanded = 1;
                } else if (strncmp(p, "lemme", 5) == 0 && !isalpha((unsigned char)p[5])) {
                    memcpy(q, "let me", 6); q += 6; p += 5; last_char = 'e'; informal_expanded = 1;
                } else if (strncmp(p, "gimme", 5) == 0 && !isalpha((unsigned char)p[5])) {
                    memcpy(q, "give me", 7); q += 7; p += 5; last_char = 'e'; informal_expanded = 1;
                } else if (strncmp(p, "ain't", 5) == 0 && !isalpha((unsigned char)p[5])) {
                    memcpy(q, "is not", 6); q += 6; p += 5; last_char = 't'; informal_expanded = 1;
                }
            }
        }
        if (informal_expanded) continue;

        // Remove repeated punctuation (e.g. "!!!" -> "!")
        int rep_punct = 0;
        if (ispunct((unsigned char)c) && (c == '!' || c == '?')) {
            while (*(p+1) == c) { p++; rep_punct = 1; }
            if (rep_punct) {
                *q++ = c;
                last_char = c;
                p++;
                continue;
            }
        }

        // Remove space before punctuation (e.g. "word ." -> "word.")
        if (q > out && isspace((unsigned char)q[-1]) && ispunct((unsigned char)c) && c != '\'') {
            q--;
        }

        *q++ = c;
        last_char = c;
        p++;

        // Track previous word for abbreviation check
        if (isalpha((unsigned char)c)) {
            if (prev_word_len < (int)sizeof(prev_word) - 1)
                prev_word[prev_word_len++] = (char)tolower((unsigned char)c);
        } else {
            prev_word_len = 0;
        }

        // Grow output buffer if needed
        if ((size_t)(q - out) > outcap - 16) {
            size_t used = q - out;
            outcap *= 2;
            char *newout = realloc(out, outcap);
            if (!newout) { free(out); return NULL; }
            out = newout;
            q = out + used;
        }
    }

    // Ensure terminal punctuation
    if (q > out && !ispunct((unsigned char)q[-1])) {
        *q++ = '.';
    }

    *q = '\0';

    // Remove trailing spaces
    while (q > out && isspace((unsigned char)q[-1])) *(--q) = 0;

    (void)ellipsis;
    (void)after_punct;

    return out;
}

/* ============================================================================
 * Readability / scoring
 * ============================================================================ */

fossil_io_soap_scores_t fossil_io_soap_score(const char *text) {
    fossil_io_soap_scores_t s = {100, 100, 100};
    if (!text) return s;

    size_t len = strlen(text);

    // Readability: penalize short or very long text
    if (len < 40) s.readability -= 40;
    else if (len > 1000) s.readability -= 10;

    // Readability: penalize long sentences
    char **sentences = fossil_io_soap_split(text);
    int long_sent = 0, total_sent = 0;
    if (sentences) {
        for (int i = 0; sentences[i]; i++) {
            size_t slen = strlen(sentences[i]);
            if (slen > 120) long_sent++;
            total_sent++;
        }
        if (total_sent > 0 && long_sent * 2 > total_sent)
            s.readability -= 10;
        for (int i = 0; sentences[i]; i++) free(sentences[i]);
        free(sentences);
    }

    // Clarity: reward line breaks, penalize excessive punctuation
    if (strchr(text, '\n')) s.clarity += 5;
    if (strstr(text, "...")) s.clarity -= 5;
    int punct_count = 0;
    for (const char *p = text; *p; p++)
        if (ispunct((unsigned char)*p)) punct_count++;
    if (punct_count > (int)(len / 6)) s.clarity -= 10;

    // Clarity: penalize repeated words
    char **words = fossil_io_soap_split(text);
    int repeated = 0;
    if (words) {
        for (int i = 1; words[i]; i++) {
            if (strcasecmp(words[i], words[i-1]) == 0) {
                repeated++;
            }
        }
        if (repeated > 0) s.clarity -= 5;
        for (int i = 0; words[i]; i++) free(words[i]);
        free(words);
    }

    // Quality: penalize excessive exclamation, reward absence of spammy patterns
    int exclam = 0;
    for (const char *p = text; *p; p++)
        if (*p == '!') exclam++;
    if (exclam > 3) s.quality -= 10;
    if (strstr(text, "!!!")) s.quality -= 10;
    if (strstr(text, "buy now") || strstr(text, "click here")) s.quality -= 10;

    // Quality: penalize all-caps words
    int allcaps = 0, wordcount = 0;
    const char *p = text;
    while (*p) {
        while (*p && isspace((unsigned char)*p)) p++;
        if (!*p) break;
        int cap = 1, lenw = 0;
        while (*p && !isspace((unsigned char)*p)) {
            if (islower((unsigned char)*p)) cap = 0;
            lenw++;
            p++;
        }
        if (cap && lenw > 2) allcaps++;
        wordcount++;
    }
    if (allcaps > 0 && allcaps * 3 > wordcount) s.quality -= 10;

    // Clamp to [0,100]
    if (s.readability > 100) s.readability = 100;
    if (s.clarity > 100) s.clarity = 100;
    if (s.quality > 100) s.quality = 100;
    if (s.readability < 0) s.readability = 0;
    if (s.clarity < 0) s.clarity = 0;
    if (s.quality < 0) s.quality = 0;

    // For very short text, also penalize clarity and quality
    if (len < 40) {
        s.clarity -= 30;
        s.quality -= 30;
        if (s.clarity < 0) s.clarity = 0;
        if (s.quality < 0) s.quality = 0;
    }

    return s;
}

const char *fossil_io_soap_readability_label(int score) {
    if (score >= 95) return "outstanding";
    if (score >= 85) return "excellent";
    if (score >= 70) return "very good";
    if (score >= 60) return "good";
    if (score >= 50) return "fair";
    if (score >= 35) return "poor";
    if (score >= 20) return "very poor";
    return "unreadable";
}

/* ============================================================================
 * Detector
 * ============================================================================ */

typedef struct {
    const char *id;
    const pattern_t *patterns;
} detector_map_t;

/* ============================================================================
 * Full detector map
 * ============================================================================ */
static const detector_map_t detector_map[] = {
    /* Document-level */
    {"conspiracy", conspiracy_patterns},

    /* Sentence-level */
    {"spam", spam_patterns},
    {"ragebait", ragebait_patterns},
    {"clickbait", clickbait_patterns},
    {"bot", bot_patterns},
    {"marketing", marketing_patterns},
    {"technobabble", technobabble_patterns},
    {"hype", hype_patterns},
    {"political", political_patterns},
    {"offensive", offensive_patterns},
    {"misinfo", misinformation_patterns},

    /* Word-level / slang */
    {"brain_rot", brain_rot_patterns},
    {"leet", NULL},       // handled separately
    {"morse", NULL},      // handled separately

    /* Stylistic / behavioral */
    {"formal", formal_patterns},
    {"casual", casual_patterns},
    {"sarcasm", sarcasm_patterns},
    {"neutral", neutral_patterns},
    {"aggressive", aggressive_patterns},
    {"emotional", emotional_patterns},
    {"passive_aggressive", passive_aggressive_patterns},
    {"snowflake", snowflake_patterns},

    /* Structural (logic handled separately) */
    {"redundant_sentences", NULL},
    {"poor_cohesion", NULL},
    {"repeated_words", NULL},

    {NULL, NULL}          // sentinel
};


/* ========================= Helper: get patterns by ID ========================= */
static const pattern_t *get_patterns(const char *detector_id) {
    for (size_t i = 0; detector_map[i].id; i++) {
        if (strcmp(detector_map[i].id, detector_id) == 0) {
            return detector_map[i].patterns;
        }
    }
    return NULL;
}

/* ============================================================================
 * Structural detection helpers
 * ============================================================================ */

static int detect_redundant_sentences(char **sentences) {
    if (!sentences) return 0;
    for (size_t i = 0; sentences[i]; i++) {
        // Normalize sentence i
        char *si = dupstr(sentences[i]);
        if (!si) continue;
        strtolower(si);
        // Remove leading/trailing whitespace and punctuation
        char *start_i = si;
        while (*start_i && (isspace((unsigned char)*start_i) || ispunct((unsigned char)*start_i))) start_i++;
        char *end_i = start_i + strlen(start_i);
        while (end_i > start_i && (isspace((unsigned char)*(end_i-1)) || ispunct((unsigned char)*(end_i-1)))) *(--end_i) = 0;

        for (size_t j = i + 1; sentences[j]; j++) {
            char *sj = dupstr(sentences[j]);
            if (!sj) continue;
            strtolower(sj);
            char *start_j = sj;
            while (*start_j && (isspace((unsigned char)*start_j) || ispunct((unsigned char)*start_j))) start_j++;
            char *end_j = start_j + strlen(start_j);
            while (end_j > start_j && (isspace((unsigned char)*(end_j-1)) || ispunct((unsigned char)*(end_j-1)))) *(--end_j) = 0;

            if (strcmp(start_i, start_j) == 0) {
                free(si);
                free(sj);
                return 1;
            }
            free(sj);
        }
        free(si);
    }
    return 0;
}

static int detect_repeated_words(char **words) {
    if (!words) return 0;
    for (size_t i = 0; words[i]; i++) {
        if (strlen(words[i]) == 0) continue;
        for (size_t j = i + 1; words[j]; j++) {
            // Ignore case and skip punctuation-only "words"
            const char *wi = words[i];
            const char *wj = words[j];
            int only_punct_i = 1, only_punct_j = 1;
            for (const char *p = wi; *p; p++) if (isalnum((unsigned char)*p)) { only_punct_i = 0; break; }
            for (const char *p = wj; *p; p++) if (isalnum((unsigned char)*p)) { only_punct_j = 0; break; }
            if (only_punct_i || only_punct_j) continue;
            if (strcasecmp(wi, wj) == 0) return 1;
        }
    }
    return 0;
}

static int detect_poor_cohesion(char **sentences) {
    if (!sentences) return 0;

    const char *linkers[] = {
        "and","but","so","because","however","therefore","thus","meanwhile",
        "moreover","furthermore","consequently","in addition","as a result",NULL
    };

    size_t total = 0;
    size_t weak = 0;

    for (size_t i = 0; sentences[i]; i++) {
        total++;

        // Count words
        size_t word_count = 0;
        char *copy = dupstr(sentences[i]);
        for (char *tok = strtok(copy, " \t\n"); tok; tok = strtok(NULL, " \t\n")) word_count++;
        free(copy);
        if (word_count < 5) weak++;

        // Check if next sentence starts with a linker (punctuation-aware)
        if (sentences[i+1]) {
            char *next = dupstr(sentences[i+1]);
            // skip leading punctuation and spaces
            char *p = next;
            while (*p && (isspace((unsigned char)*p) || ispunct((unsigned char)*p))) p++;

            int has_linker = 0;
            for (size_t k = 0; linkers[k]; k++) {
                size_t len = strlen(linkers[k]);
                if (strncasecmp(p, linkers[k], len) == 0 &&
                    (isspace((unsigned char)p[len]) || ispunct((unsigned char)p[len]) || p[len]==0)) {
                    has_linker = 1;
                    break;
                }
            }
            if (!has_linker) weak++;
            free(next);
        }
    }

    if (total > 2) {
        size_t rep = 0;
        for (size_t i = 1; sentences[i]; i++) {
            char *first1 = dupstr(sentences[i-1]);
            char *first2 = dupstr(sentences[i]);
            char *w1 = strtok(first1, " \t\n");
            char *w2 = strtok(first2, " \t\n");
            if (w1 && w2 && strcasecmp(w1, w2) == 0) rep++;
            free(first1);
            free(first2);
        }
        if (((double)rep / (total-1)) > 0.4) weak++;
    }

    size_t short_count = 0, long_count = 0;
    for (size_t i = 0; sentences[i]; i++) {
        size_t wc = 0;
        char *copy = dupstr(sentences[i]);
        for (char *tok = strtok(copy, " \t\n"); tok; tok = strtok(NULL, " \t\n")) wc++;
        free(copy);
        if (wc < 4) short_count++;
        if (wc > 25) long_count++;
    }
    if (total > 0 && ((double)short_count / total) > 0.5) weak++;
    if (total > 0 && ((double)long_count / total) > 0.3) weak++;

    if (total == 0) return 0;
    return ((double)weak / total) > 0.3;
}

/* ============================================================================
 * Word-level helpers
 * ============================================================================ */
static int match_brain_rot(const char *word) {
    for (int i=0; brain_rot_patterns[i].pattern; i++) {
        if (strstr(word, brain_rot_patterns[i].pattern)) return 1;
    }
    return 0;
}

/* ============================================================================
 * Refactored fossil_io_soap_detect with Morse, BrainRot, Leet, and Structural
 * ============================================================================ */
/* Helper: match_patterns implementation */
static int match_patterns(const char *text, const pattern_t *patterns) {
    if (!text || !patterns) return 0;
    for (int i = 0; patterns[i].pattern; i++) {
        if (strstr(text, patterns[i].pattern)) return 1;
    }
    return 0;
}

int fossil_io_soap_detect(const char *text, const char *detector_id) {
    if (!text || !detector_id) return 0;

    int result = 0;

    // Document-level normalization using advanced leet normalization
    char *norm = dupstr(text);
    normalize_leet(norm);
    strtolower(norm);

    const pattern_t *patterns = get_patterns(detector_id);
    if (patterns && match_patterns(norm, patterns)) result = 1;
    free(norm);

    // Sentence-level detection
    char **sentences = fossil_io_soap_split(text);
    if (sentences && !result) {
        for (size_t i = 0; sentences[i]; i++) {
            char *s_norm = dupstr(sentences[i]);
            normalize_leet(s_norm);
            strtolower(s_norm);

            if (patterns && match_patterns(s_norm, patterns)) {
                result = 1;
                free(s_norm);
                break;
            }
            free(s_norm);
        }
    }

    // Word-level detection
    char **words = fossil_io_soap_split(text);
    if (words && !result) {
        for (size_t i = 0; words[i]; i++) {
            char *w_norm = dupstr(words[i]);
            normalize_leet(w_norm);
            strtolower(w_norm);

            if (strcmp(detector_id, "brain_rot") == 0) {
                if (match_brain_rot(w_norm)) {
                    result = 1;
                    free(w_norm);
                    break;
                }
            } else if (strcmp(detector_id, "leet") == 0) {
                // Use advanced leet detection: check if leet_map_adv would change the word
                int leet_found = 0;
                char *orig = dupstr(words[i]);
                char *test = dupstr(words[i]);
                normalize_leet(test);
                if (strcmp(orig, test) != 0) leet_found = 1;
                free(orig);
                free(test);
                if (leet_found) {
                    result = 1;
                    free(w_norm);
                    break;
                }
            } else if (strcmp(detector_id, "morse") == 0) {
                char *decoded = decode_morse(w_norm);
                if (decoded) {
                    for (size_t j = 0; j < strlen(decoded); j++) {
                        if (isalpha((unsigned char)decoded[j])) {
                            result = 1;
                            break;
                        }
                    }
                    free(decoded);
                }
                if (result) {
                    free(w_norm);
                    break;
                }
            }

            if (!result && patterns && match_patterns(w_norm, patterns)) {
                result = 1;
                free(w_norm);
                break;
            }

            free(w_norm);
        }
    }

    // Structural detection
    if (!result) {
        if (strcmp(detector_id, "redundant_sentences") == 0) {
            result = detect_redundant_sentences(sentences);
        } else if (strcmp(detector_id, "repeated_words") == 0) {
            result = detect_repeated_words(words);
        } else if (strcmp(detector_id, "poor_cohesion") == 0) {
            result = detect_poor_cohesion(sentences);
        }
    }

    // Cleanup
    if (sentences) {
        for (size_t i = 0; sentences[i]; i++) free(sentences[i]);
        free(sentences);
    }

    if (words) {
        for (size_t i = 0; words[i]; i++) free(words[i]);
        free(words);
    }

    return result;
}

/* ============================================================================
 * Split / Reflow / Capitalize
 * ============================================================================ */

char **fossil_io_soap_split(const char *text) {
    // Enhanced: split as sentences if sentence-ending punctuation is present, else split as words.
    if (!text) return NULL;

    // Heuristic: if text contains '.', '!' or '?', treat as sentences
    int is_sentence = 0;
    for (const char *q = text; *q; q++) {
        if (*q == '.' || *q == '!' || *q == '?') { is_sentence = 1; break; }
    }

    // First pass: count segments
    size_t count = 0;
    const char *p = text;
    int in_token = 0;
    while (*p) {
        if (!is_sentence) {
            if (!isspace((unsigned char)*p) && !in_token) {
                in_token = 1;
                count++;
            } else if (isspace((unsigned char)*p)) {
                in_token = 0;
            }
        } else {
            if (!in_token && !isspace((unsigned char)*p)) {
                in_token = 1;
                count++;
            }
            if (is_sentence && (*p == '.' || *p == '!' || *p == '?')) {
                in_token = 0;
            }
        }
        p++;
    }

    char **arr = (char**)malloc(sizeof(char*) * (count + 1));
    if (!arr) return NULL;
    size_t idx = 0;

    // Second pass: extract segments
    p = text;
    while (*p) {
        // Skip leading whitespace and control chars
        while (*p && (isspace((unsigned char)*p) || ((unsigned char)*p < 32 && *p != '\n'))) p++;
        if (!*p) break;
        const char *start = p;
        if (!is_sentence) {
            // Word mode
            while (*p && !isspace((unsigned char)*p) && ((unsigned char)*p >= 32 || *p == '\n')) p++;
        } else {
            // Sentence mode
            int in_quote = 0;
            while (*p) {
                if (*p == '"' || *p == '\'') in_quote ^= 1;
                if (!in_quote && (*p == '.' || *p == '!' || *p == '?')) {
                    p++; // include punctuation
                    break;
                }
                p++;
            }
        }
        size_t len = p - start;
        // Trim trailing whitespace and control chars
        while (len > 0 && (isspace((unsigned char)start[len-1]) || ((unsigned char)start[len-1] < 32 && start[len-1] != '\n'))) len--;
        if (len > 0) {
            char *s = (char*)malloc(len + 1);
            if (s) {
                strncpy(s, start, len);
                s[len] = 0;
                arr[idx++] = s;
            }
        }
    }
    arr[idx] = NULL;
    return arr;
}

char *fossil_io_soap_reflow(const char *text, int width) {
    if (!text || width <= 0) return dupstr(text);

    size_t len = strlen(text);
    char *out = malloc(len * 2 + 2);
    if (!out) return NULL;

    int col = 0;
    size_t oi = 0;
    const char *p = text;
    int last_space_oi = -1;
    int last_space_col = 0;

    while (*p) {
        if (*p == '\n') {
            out[oi++] = *p++;
            col = 0;
            last_space_oi = -1;
            continue;
        }
        if (isspace((unsigned char)*p)) {
            // collapse multiple spaces/tabs to a single space
            if (oi > 0 && out[oi-1] != ' ' && out[oi-1] != '\n') {
                out[oi++] = ' ';
                col++;
                last_space_oi = (int)oi - 1;
                last_space_col = col - 1;
            }
            // skip all consecutive whitespace
            while (isspace((unsigned char)*p)) ++p;
            continue;
        }
        out[oi++] = *p++;
        col++;
        if (col >= width) {
            // Try to break at last space
            if (last_space_oi >= 0) {
                out[last_space_oi] = '\n';
                // recalc col: chars since last_space_oi
                col = (int)(oi - last_space_oi - 1);
                last_space_oi = -1;
            } else {
                out[oi++] = '\n';
                col = 0;
            }
        }
        // update last_space_oi if this char is a space (for next round)
        if (oi > 0 && out[oi-1] == ' ') {
            last_space_oi = (int)oi - 1;
            last_space_col = col - 1;
        }
    }
    // Remove trailing space before final newline
    while (oi > 0 && out[oi-1] == ' ') oi--;
    out[oi] = 0;
    return out;
}

char *fossil_io_soap_capitalize(const char *text, int mode) {
    if (!text) return NULL;
    char *s = dupstr(text);
    if (!s) return NULL;

    if (mode == 0) {
        // Capitalize the first letter of each sentence (after ., !, ?)
        int cap = 1;
        int in_quote = 0;
        for (char *p = s; *p; p++) {
            if (*p == '"' || *p == '\'') in_quote ^= 1;
            if (cap && isalpha((unsigned char)*p)) {
                *p = (char)toupper((unsigned char)*p);
                cap = 0;
            }
            // Sentence end: ., !, ? (not inside quotes)
            if (!in_quote && (*p == '.' || *p == '!' || *p == '?')) {
                // Look ahead for ellipsis
                if (*p == '.' && *(p+1) == '.' && *(p+2) == '.') {
                    p += 2; // skip ellipsis
                }
                // Only set cap if not at end of string
                if (*(p+1)) cap = 1;
            }
            // Also capitalize after line breaks
            if (*p == '\n') cap = 1;
        }
    } else if (mode == 1) {
        // Capitalize the first letter of each word
        int cap = 1;
        for (char *p = s; *p; p++) {
            if (isspace((unsigned char)*p) || ispunct((unsigned char)*p)) {
                cap = 1;
            } else if (cap && isalpha((unsigned char)*p)) {
                *p = (char)toupper((unsigned char)*p);
                cap = 0;
            }
        }
    }
    return s;
}

/* ============================================================================
 * Suggest / Summarize
 * ============================================================================ */

char *fossil_io_soap_suggest(const char *text) {
    if (!text) return NULL;

    char *norm = fossil_io_soap_sanitize(text);
    if (!norm) return NULL;

    char *corrected = fossil_io_soap_correct_grammar(norm);
    free(norm);
    if (!corrected) return NULL;

    // Collapse multiple spaces, but preserve trailing space if present in input
    size_t len = strlen(corrected);
    char *out = malloc(len + 2);
    if (!out) { free(corrected); return NULL; }
    char *p = corrected, *q = out;
    int last_space = 0;
    while (*p) {
        if (isspace((unsigned char)*p)) {
            if (!last_space) *q++ = ' ';
            last_space = 1;
        } else {
            *q++ = *p;
            last_space = 0;
        }
        p++;
    }
    *q = 0;

    // If input had trailing space, preserve it
    if (len > 0 && isspace((unsigned char)corrected[len-1]) && q > out && out[q-out-1] != ' ') {
        *q++ = ' ';
        *q = 0;
    }

    free(corrected);

    // Capitalize the first letter of each sentence
    char *final = fossil_io_soap_capitalize(out, 0);
    free(out);

    return final;
}

char *fossil_io_soap_summarize(const char *text) {
    if (!text) return NULL;
    size_t textlen = strlen(text);
    if (textlen == 0) return dupstr("");

    char **sentences = fossil_io_soap_split(text);
    if (!sentences) return dupstr(text);

    // Count non-empty sentences
    int nonempty = 0;
    for (int i = 0; sentences[i]; i++) {
        char *s = sentences[i];
        int non_ws = 0;
        for (char *p = s; *p; p++) if (!isspace((unsigned char)*p)) { non_ws = 1; break; }
        if (non_ws) nonempty++;
    }

    // If only one non-empty sentence, return it as summary
    if (nonempty == 1) {
        char *result = NULL;
        for (int i = 0; sentences[i]; i++) {
            char *s = sentences[i];
            int non_ws = 0;
            for (char *p = s; *p; p++) if (!isspace((unsigned char)*p)) { non_ws = 1; break; }
            if (non_ws) {
                result = dupstr(s);
                break;
            }
        }
        for (int i = 0; sentences[i]; i++) free(sentences[i]);
        free(sentences);
        return result ? result : dupstr("");
    }

    // Otherwise, summarize with up to two non-empty sentences
    int max_sentences = 2;
    char *out = malloc(1024);
    if (!out) {
        for (int i = 0; sentences[i]; i++) free(sentences[i]);
        free(sentences);
        return NULL;
    }
    out[0] = 0;

    int count = 0;
    for (int i = 0; sentences[i] && count < max_sentences; i++) {
        // Skip empty or whitespace-only sentences
        char *s = sentences[i];
        int non_ws = 0;
        for (char *p = s; *p; p++) if (!isspace((unsigned char)*p)) { non_ws = 1; break; }
        if (!non_ws) continue;

        // Add sentence to summary
        strcat(out, s);
        size_t slen = strlen(s);
        if (slen == 0 || (s[slen-1] != '.' && s[slen-1] != '!' && s[slen-1] != '?'))
            strcat(out, ".");
        strcat(out, " ");
        count++;
    }

    // If no sentences found, fallback to first 80 chars
    if (count == 0) {
        strncpy(out, text, 80);
        out[80] = 0;
    }

    // Cleanup
    for (int i = 0; sentences[i]; i++) free(sentences[i]);
    free(sentences);

    // Trim trailing space
    size_t len = strlen(out);
    while (len > 0 && isspace((unsigned char)out[len-1])) out[--len] = 0;

    return out;
}

/* ============================================================================
 * Rewrite & Format
 * ============================================================================ */

// Rewrite: attempts to rephrase text by correcting grammar, normalizing style, and reflowing lines.
char *fossil_io_soap_rewrite(const char *text) {
    if (!text) return NULL;

    // Sanitize and normalize
    char *norm = fossil_io_soap_sanitize(text);
    if (!norm) return NULL;

    // Correct grammar
    char *corrected = fossil_io_soap_correct_grammar(norm);
    free(norm);
    if (!corrected) return NULL;

    // Capitalize sentences
    char *capitalized = fossil_io_soap_capitalize(corrected, 0);
    free(corrected);
    if (!capitalized) return NULL;

    // Reflow to 80 columns
    char *reflowed = fossil_io_soap_reflow(capitalized, 80);
    free(capitalized);

    return reflowed;
}

// Format: pretty-prints text with consistent indentation and line breaks.
char *fossil_io_soap_format(const char *text) {
    if (!text) return NULL;

    // Remove excessive whitespace and normalize
    char *norm = fossil_io_soap_sanitize(text);
    if (!norm) return NULL;

    // Capitalize sentences
    char *capitalized = fossil_io_soap_capitalize(norm, 0);
    free(norm);
    if (!capitalized) return NULL;

    // Reflow to 72 columns for formatting
    char *formatted = fossil_io_soap_reflow(capitalized, 72);
    free(capitalized);

    return formatted;
}
